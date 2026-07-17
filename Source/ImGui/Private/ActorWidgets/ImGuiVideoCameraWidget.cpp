#include "ActorWidgets/ImGuiVideoCameraWidget.h"
#include "CameraRecorderComponent.h"
#include "StorageCardComponent.h"
#include "MicrophoneRecorderComponent.h"
#include "BatteryConsumerComponent.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <UObject/UObjectIterator.h>

static FString GetVideoCameraNetRoleString(ENetRole Role)
{
	switch (Role)
	{
		case ROLE_None: return TEXT("None");
		case ROLE_SimulatedProxy: return TEXT("Simulated Proxy");
		case ROLE_AutonomousProxy: return TEXT("Autonomous Proxy");
		case ROLE_Authority: return TEXT("Authority");
		default: return TEXT("Unknown");
	}
}

static FString GetVideoCameraNetDormancyString(ENetDormancy Dormancy)
{
	switch (Dormancy)
	{
		case DORM_Never: return TEXT("Never Dormant (Awake)");
		case DORM_Awake: return TEXT("Awake");
		case DORM_DormantAll: return TEXT("Dormant All");
		case DORM_DormantPartial: return TEXT("Dormant Partial");
		case DORM_Initial: return TEXT("Initial Dormant");
		default: return TEXT("Unknown");
	}
}

namespace ImGuiVideoCameraWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Video Cameras Debug", nullptr, ImGuiWindowFlags_None))
		{
			ImGui::End();
			return;
		}

		if (!World)
		{
			ImGui::Text("Invalid World");
			ImGui::End();
			return;
		}

		int32 CameraCount = 0;

		for (TObjectIterator<UCameraRecorderComponent> It; It; ++It)
		{
			UCameraRecorderComponent* CamComp = *It;
			if (!IsValid(CamComp) || CamComp->GetWorld() != World || CamComp->IsTemplate())
			{
				continue;
			}

			CameraCount++;

			AActor* OwnerActor = CamComp->GetOwner();
			FString CameraName = OwnerActor ? OwnerActor->GetName() : CamComp->GetName();
			FString CameraHeader = FString::Printf(TEXT("Camera %d: %s"), CameraCount, *CameraName);

			if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*CameraHeader), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("CameraProps", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Recording State");
					ImGui::TableSetColumnIndex(1);
					ERecordingState State = CamComp->GetRecordingState();
					const char* StateStr = "Unknown";
					ImVec4 StateColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
					switch (State)
					{
						case ERecordingState::Idle: StateStr = "Idle"; StateColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); break;
						case ERecordingState::Recording: StateStr = "Recording"; StateColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f); break;
						case ERecordingState::Finalizing: StateStr = "Finalizing"; StateColor = ImVec4(0.9f, 0.6f, 0.2f, 1.0f); break;
						case ERecordingState::Failed: StateStr = "Failed"; StateColor = ImVec4(0.9f, 0.1f, 0.1f, 1.0f); break;
					}
					ImGui::TextColored(StateColor, "%s", StateStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Elapsed Time");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f seconds (Max: %.1fs)", CamComp->GetRecordingElapsedSeconds(), CamComp->MaxRecordingDurationSeconds);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Recording FPS");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%d", CamComp->RecordingFPS);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Quality Preset");
					ImGui::TableSetColumnIndex(1);
					const char* PresetStr = "Unknown";
					switch (CamComp->QualityPreset)
					{
						case ECameraQualityPreset::Low: PresetStr = "Low (480p)"; break;
						case ECameraQualityPreset::Standard: PresetStr = "Standard (720p)"; break;
						case ECameraQualityPreset::High: PresetStr = "High (1080p)"; break;
						case ECameraQualityPreset::Ultra2K: PresetStr = "Ultra (2K)"; break;
					}
					ImGui::Text("%s", PresetStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Focus Method");
					ImGui::TableSetColumnIndex(1);
					ERecordingCameraFocusMethod FocusMethod = CamComp->GetFocusMethod();
					ImGui::Text("%s", FocusMethod == ERecordingCameraFocusMethod::Manual ? "Manual" : "None (Infinity)");

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Focus Distance");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f cm", CamComp->GetFocusDistance());

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Aperture");
					ImGui::TableSetColumnIndex(1); ImGui::Text("f/%.1f", UCameraRecorderComponent::ApertureToFStop(CamComp->GetAperture()));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("ISO");
					ImGui::TableSetColumnIndex(1); ImGui::Text("EV %.1f", UCameraRecorderComponent::ISOToExposureCompensation(CamComp->GetISO()));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("White Balance");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.0f K", UCameraRecorderComponent::WhiteBalanceToKelvin(CamComp->GetWhiteBalance()));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Field of View (Zoom)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f degrees", CamComp->GetFieldOfView());

					UBatteryConsumerComponent* BatteryConsumer = OwnerActor ? OwnerActor->FindComponentByClass<UBatteryConsumerComponent>() : nullptr;
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Battery Consumer");
					ImGui::TableSetColumnIndex(1);
					if (BatteryConsumer)
					{
						if (BatteryConsumer->HasBattery())
						{
							float Pct = BatteryConsumer->GetBatteryChargePercent();
							char ProgressBarBuf[64];
							snprintf(ProgressBarBuf, sizeof(ProgressBarBuf), "%.1f%% (%.1f / %.1f)", Pct, BatteryConsumer->GetBatteryCurrentCharge(), BatteryConsumer->GetBatteryMaxCapacity());
							ImGui::ProgressBar(Pct / 100.f, ImVec2(-FLT_MIN, 0.0f), ProgressBarBuf);
						}
						else
						{
							ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "No Battery Slotted");
						}
					}
					else
					{
						ImGui::Text("Not Installed");
					}

					UStorageCardComponent* BoundCard = CamComp->GetBoundStorageCardComponent();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Bound Storage Card");
					ImGui::TableSetColumnIndex(1);
					if (BoundCard)
					{
						ImGui::Text("%s (ID: %s)", TCHAR_TO_UTF8(*BoundCard->CardDisplayName), TCHAR_TO_UTF8(*BoundCard->CardID.ToString().Left(8)));
					}
					else
					{
						ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "None");
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Attached Microphone");
					ImGui::TableSetColumnIndex(1);
					UMicrophoneRecorderComponent* AttachedMic = nullptr;
					if (OwnerActor)
					{
						TArray<AActor*> AttachedActors;
						OwnerActor->GetAttachedActors(AttachedActors);
						for (AActor* AttachedActor : AttachedActors)
						{
							if (IsValid(AttachedActor))
							{
								AttachedMic = AttachedActor->FindComponentByClass<UMicrophoneRecorderComponent>();
								if (AttachedMic)
								{
									break;
								}
							}
						}
					}
					if (AttachedMic)
					{
						AActor* MicOwner = AttachedMic->GetOwner();
						ImGui::Text("%s", MicOwner ? TCHAR_TO_UTF8(*MicOwner->GetName()) : "Yes");
					}
					else
					{
						ImGui::Text("None");
					}

					if (CamComp->IsRecording())
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Active Session ID");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*CamComp->GetActiveFileID().ToString()));
					}

					if (OwnerActor)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetVideoCameraNetRoleString(OwnerActor->GetLocalRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Dormancy");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetVideoCameraNetDormancyString(OwnerActor->NetDormancy)));
					}

					ImGui::EndTable();
				}
				ImGui::Unindent();
			}
		}

		if (CameraCount == 0)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "No active Video Cameras found in the level.");
		}

		ImGui::End();
	}
}
