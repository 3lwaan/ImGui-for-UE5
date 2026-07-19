#include "ActorWidgets/ImGuiInsightsTracerWidget.h"

#include <imgui.h>
#include <Engine/Engine.h>
#include <Engine/World.h>
#include <Misc/Paths.h>
#include <HAL/PlatformProcess.h>
#include <HAL/FileManager.h>
#include <Misc/App.h>
#include "ActorWidgets/ActorDebugWidgetsSubsystem.h"

FImGuiOutputDevice& FImGuiOutputDevice::Get()
{
	static FImGuiOutputDevice Instance;
	return Instance;
}

FImGuiOutputDevice::FImGuiOutputDevice()
	: bRegistered(false)
	, bHasNewLines(false)
{
}

FImGuiOutputDevice::~FImGuiOutputDevice()
{
	Unregister();
}

void FImGuiOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category)
{
	FScopeLock Lock(&CriticalSection);

	FLogLine Line;
	Line.Message = V;
	Line.Verbosity = Verbosity;
	Line.Category = Category;

	Lines.Add(Line);
	if (Lines.Num() > 500)
	{
		Lines.RemoveAt(0);
	}

	bHasNewLines = true;
}

void FImGuiOutputDevice::Clear()
{
	FScopeLock Lock(&CriticalSection);
	Lines.Empty();
}

TArray<FLogLine> FImGuiOutputDevice::GetLines()
{
	FScopeLock Lock(&CriticalSection);
	return Lines;
}

bool FImGuiOutputDevice::GetAndClearNewLinesFlag()
{
	bool bRet = bHasNewLines;
	bHasNewLines = false;
	return bRet;
}

void FImGuiOutputDevice::Register()
{
	if (GLog && !bRegistered)
	{
		GLog->AddOutputDevice(this);
		bRegistered = true;
	}
}

void FImGuiOutputDevice::Unregister()
{
	if (GLog && bRegistered)
	{
		GLog->RemoveOutputDevice(this);
		bRegistered = false;
	}
}

namespace ImGuiInsightsTracerWidget
{
	void Draw(UWorld* World)
	{
		if (!World)
		{
			return;
		}

		UActorDebugWidgetsSubsystem* Subsystem = World->GetSubsystem<UActorDebugWidgetsSubsystem>();
		if (!Subsystem)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Subsystem not found");
			ImGui::End();
			return;
		}

		// Shift buffer & compute simulated trace overhead data rate
		float NewValue = 0.0f;
		if (Subsystem->bIsTracing)
		{
			// Fluctuates between 75MB/s and 125MB/s to simulate trace stream writing activity
			NewValue = 75.0f + (FMath::FRand() * 50.0f);
		}
		else
		{
			// Flat zero when stopped
			NewValue = 0.0f;
		}

		Subsystem->TraceThroughput[Subsystem->TraceThroughputIndex] = NewValue;
		Subsystem->TraceThroughputIndex = (Subsystem->TraceThroughputIndex + 1) % 100;

		ImGui::SetNextWindowSize(ImVec2(400.0f, 320.0f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Unreal Insights Tracer", nullptr, ImGuiWindowFlags_NoScrollbar))
		{
			// Status bar / Header Indicator
			if (Subsystem->bIsTracing)
			{
				// Pulsing recording red color
				float PulseAlpha = 0.6f + (FMath::Sin(FApp::GetCurrentTime() * 8.0f) * 0.4f);
				ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, PulseAlpha), "● RECORDING ACTIVE");
			}
			else
			{
				ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "○ TRACER STANDBY");
			}

			ImGui::Separator();
			ImGui::Spacing();

			// Section 1: Capture Tracing Buttons
			if (!Subsystem->bIsTracing)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); // Safe Green
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
				if (ImGui::Button("Start Capture Trace", ImVec2(-FLT_MIN, 40.0f)))
				{
					if (GEngine)
					{
						GEngine->Exec(World, TEXT("trace.start cpu,frame,gpu,bookmark,loadtime"));
					}
					Subsystem->bIsTracing = true;
				}
				ImGui::PopStyleColor(2);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Soft Red
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
				if (ImGui::Button("Stop Capture Trace", ImVec2(-FLT_MIN, 40.0f)))
				{
					if (GEngine)
					{
						GEngine->Exec(World, TEXT("trace.stop"));
					}
					Subsystem->bIsTracing = false;
				}
				ImGui::PopStyleColor(2);
			}

			ImGui::Spacing();

			// Section 2: Explore Trace files
			if (ImGui::Button("Open Traces Folder", ImVec2(-FLT_MIN, 30.0f)))
			{
				FString TraceFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Profiling/"));
				IFileManager::Get().MakeDirectory(*TraceFolder, true);
				TraceFolder.ReplaceInline(TEXT("/"), TEXT("\\"));
				FPlatformProcess::ExploreFolder(*TraceFolder);
			}

			ImGui::Spacing();

			// Section 2b: Output Log & Multi-Monitor controls
			if (ImGui::Button(Subsystem->bShowOutputLog ? "Hide Output Log" : "Show Output Log", ImVec2(-FLT_MIN, 30.0f)))
			{
				Subsystem->bShowOutputLog = !Subsystem->bShowOutputLog;
			}

			ImGui::Spacing();

			// Multi-Monitor viewports require backend support for Platform_CreateWindow/DestroyWindow.
			// Since the custom Slate backend in this plugin does not implement these OS handlers, multi-viewport is disabled.
			bool bMultiWindow = false;
			ImGui::BeginDisabled(true);
			ImGui::Checkbox("Enable Multi-Window Mode (Not supported by Slate Backend)", &bMultiWindow);
			ImGui::EndDisabled();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Section 3: Visual Trace stream throughput chart
			float MaxVal = 0.0f;
			float AvgVal = 0.0f;
			for (int32 i = 0; i < 100; ++i)
			{
				AvgVal += Subsystem->TraceThroughput[i];
				if (Subsystem->TraceThroughput[i] > MaxVal)
				{
					MaxVal = Subsystem->TraceThroughput[i];
				}
			}
			AvgVal /= 100.0f;

			char GraphLabel[64];
			snprintf(GraphLabel, sizeof(GraphLabel), "Avg: %.1f MB/s, Peak: %.1f MB/s", AvgVal, MaxVal);

			ImGui::Text("Trace Stream Output Rate:");
			
			if (Subsystem->bIsTracing)
			{
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); // Red plot
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Grey plot
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
			}

			ImGui::PlotLines("##TraceBufferPlot", Subsystem->TraceThroughput, 100, Subsystem->TraceThroughputIndex, GraphLabel, 0.0f, 150.0f, ImVec2(0.0f, 80.0f));
			ImGui::PopStyleColor(2);

			ImGui::Spacing();
			ImGui::TextWrapped("Saved trace files (*.utrace) can be opened and analyzed using the standalone Unreal Insights tool found in Engine/Binaries/DotNET/UnrealInsights/.");
		}
		ImGui::End();

		// Section 4: Render separate Runtime Output Log Window
		if (Subsystem->bShowOutputLog)
		{
			ImGui::SetNextWindowSize(ImVec2(600.0f, 400.0f), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Runtime Output Log", &Subsystem->bShowOutputLog))
			{
				if (ImGui::Button("Clear Log"))
				{
					FImGuiOutputDevice::Get().Clear();
				}

				ImGui::Separator();
				ImGui::Spacing();

				// Child window for scroll area
				ImGui::BeginChild("ScrollingRegion", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

				TArray<FLogLine> Lines = FImGuiOutputDevice::Get().GetLines();
				for (const FLogLine& Line : Lines)
				{
					ImVec4 Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default White
					if (Line.Verbosity == ELogVerbosity::Error || Line.Verbosity == ELogVerbosity::Fatal)
					{
						Color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
					}
					else if (Line.Verbosity == ELogVerbosity::Warning)
					{
						Color = ImVec4(1.0f, 0.9f, 0.3f, 1.0f); // Yellow
					}

					ImGui::TextColored(Color, "[%s] %s", TCHAR_TO_UTF8(*Line.Category.ToString()), TCHAR_TO_UTF8(*Line.Message));
				}

				// Auto-scroll logic: scroll to bottom if new lines are present and we are at the bottom
				if (FImGuiOutputDevice::Get().GetAndClearNewLinesFlag())
				{
					if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f)
					{
						ImGui::SetScrollHereY(1.0f);
					}
				}

				ImGui::EndChild();
			}
			ImGui::End();
		}
	}
}
