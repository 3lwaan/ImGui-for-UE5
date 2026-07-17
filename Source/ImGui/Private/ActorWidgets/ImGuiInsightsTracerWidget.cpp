#include "ActorWidgets/ImGuiInsightsTracerWidget.h"

#include <imgui.h>
#include <Engine/Engine.h>
#include <Engine/World.h>
#include <Misc/Paths.h>
#include <HAL/PlatformProcess.h>
#include <HAL/FileManager.h>
#include <Misc/App.h>

namespace ImGuiInsightsTracerWidget
{
	void Draw(UWorld* World)
	{
		if (!World)
		{
			return;
		}

		static bool bIsTracing = false;

		// Simulated tracing buffer throughput plot
		static float TraceThroughput[100] = { 0.0f };
		static int32 TraceThroughputIndex = 0;

		// Shift buffer & compute simulated trace overhead data rate
		float NewValue = 0.0f;
		if (bIsTracing)
		{
			// Fluctuates between 75MB/s and 125MB/s to simulate trace stream writing activity
			NewValue = 75.0f + (FMath::FRand() * 50.0f);
		}
		else
		{
			// Flat zero when stopped
			NewValue = 0.0f;
		}

		TraceThroughput[TraceThroughputIndex] = NewValue;
		TraceThroughputIndex = (TraceThroughputIndex + 1) % 100;

		ImGui::SetNextWindowSize(ImVec2(400.0f, 320.0f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Unreal Insights Tracer", nullptr, ImGuiWindowFlags_NoScrollbar))
		{
			// Status bar / Header Indicator
			if (bIsTracing)
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
			if (!bIsTracing)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); // Safe Green
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
				if (ImGui::Button("Start Capture Trace", ImVec2(-FLT_MIN, 40.0f)))
				{
					if (GEngine)
					{
						GEngine->Exec(World, TEXT("trace.start cpu,frame,gpu,bookmark,loadtime"));
					}
					bIsTracing = true;
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
					bIsTracing = false;
				}
				ImGui::PopStyleColor(2);
			}

			ImGui::Spacing();

			// Section 2: Explore Trace files
			if (ImGui::Button("Open Traces Folder", ImVec2(-FLT_MIN, 30.0f)))
			{
				FString TraceFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Profiling/Traces/"));
				IFileManager::Get().MakeDirectory(*TraceFolder, true);
				TraceFolder.ReplaceInline(TEXT("/"), TEXT("\\"));
				FPlatformProcess::ExploreFolder(*TraceFolder);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Section 3: Visual Trace stream throughput chart
			float MaxVal = 0.0f;
			float AvgVal = 0.0f;
			for (int32 i = 0; i < 100; ++i)
			{
				AvgVal += TraceThroughput[i];
				if (TraceThroughput[i] > MaxVal)
				{
					MaxVal = TraceThroughput[i];
				}
			}
			AvgVal /= 100.0f;

			char GraphLabel[64];
			snprintf(GraphLabel, sizeof(GraphLabel), "Avg: %.1f MB/s, Peak: %.1f MB/s", AvgVal, MaxVal);

			ImGui::Text("Trace Stream Output Rate:");
			
			if (bIsTracing)
			{
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); // Red plot
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Grey plot
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
			}

			ImGui::PlotLines("##TraceBufferPlot", TraceThroughput, 100, TraceThroughputIndex, GraphLabel, 0.0f, 150.0f, ImVec2(0.0f, 80.0f));
			ImGui::PopStyleColor(2);

			ImGui::Spacing();
			ImGui::TextWrapped("Saved trace files (*.utrace) can be opened and analyzed using the standalone Unreal Insights tool found in Engine/Binaries/DotNET/UnrealInsights/.");
		}
		ImGui::End();
	}
}
