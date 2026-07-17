#include "ActorWidgets/ImGuiAudioMicrophoneWidget.h"
#include "MicrophoneRecorderComponent.h"
#include "StorageCardComponent.h"
#include "CameraRecorderComponent.h"
#include "BatteryConsumerComponent.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <UObject/UObjectIterator.h>

static FString GetAudioMicrophoneNetRoleString(ENetRole Role)
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

static FString GetAudioMicrophoneNetDormancyString(ENetDormancy Dormancy)
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

namespace ImGuiAudioMicrophoneWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Audio Microphones Debug", nullptr, ImGuiWindowFlags_None))
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

		int32 MicCount = 0;

		for (TObjectIterator<UMicrophoneRecorderComponent> It; It; ++It)
		{
			UMicrophoneRecorderComponent* MicComp = *It;
			if (!IsValid(MicComp) || MicComp->GetWorld() != World || MicComp->IsTemplate())
			{
				continue;
			}

			MicCount++;

			AActor* OwnerActor = MicComp->GetOwner();
			FString MicName = OwnerActor ? OwnerActor->GetName() : MicComp->GetName();
			FString MicHeader = FString::Printf(TEXT("Microphone %d: %s"), MicCount, *MicName);

			if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*MicHeader), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("MicProps", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Recording State");
					ImGui::TableSetColumnIndex(1);
					EAudioRecordingState State = MicComp->GetRecordingState();
					const char* StateStr = "Unknown";
					ImVec4 StateColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
					switch (State)
					{
						case EAudioRecordingState::Idle: StateStr = "Idle"; StateColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); break;
						case EAudioRecordingState::Recording: StateStr = "Recording"; StateColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f); break;
						case EAudioRecordingState::Finalizing: StateStr = "Finalizing"; StateColor = ImVec4(0.9f, 0.6f, 0.2f, 1.0f); break;
						case EAudioRecordingState::Failed: StateStr = "Failed"; StateColor = ImVec4(0.9f, 0.1f, 0.1f, 1.0f); break;
					}
					ImGui::TextColored(StateColor, "%s", StateStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Elapsed Time");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f seconds (Max: %.1fs)", MicComp->GetRecordingElapsedSeconds(), MicComp->MaxRecordingDurationSeconds);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Gain");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f", MicComp->MicGain);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("High Pass Filter");
					ImGui::TableSetColumnIndex(1);
					const char* HpfStr = "Unknown";
					switch (MicComp->HighPassFilter)
					{
						case EMicHighPassFilter::Off: HpfStr = "Off"; break;
						case EMicHighPassFilter::Hz80: HpfStr = "80 Hz (Wind/Rumble)"; break;
						case EMicHighPassFilter::Hz120: HpfStr = "120 Hz (Standard Boom)"; break;
						case EMicHighPassFilter::Hz200: HpfStr = "200 Hz (Aggressive)"; break;
					}
					ImGui::Text("%s", HpfStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Pickup Range");
					ImGui::TableSetColumnIndex(1);
					const char* RangeStr = "Unknown";
					switch (MicComp->PickupRange)
					{
						case EMicPickupRange::Close: RangeStr = "Close (5m)"; break;
						case EMicPickupRange::Normal: RangeStr = "Normal (20m)"; break;
						case EMicPickupRange::Wide: RangeStr = "Wide (50m)"; break;
					}
					ImGui::Text("%s", RangeStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Operating Mode");
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", MicComp->bRequiresDirectPowerAndStorage ? "Freestanding (Requires Local Battery/Card)" : "Accessory Mode (Requires Attached Camera)");

					UCameraRecorderComponent* AttachedCam = MicComp->GetAttachedCameraRecorder();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Attached Camera");
					ImGui::TableSetColumnIndex(1);
					if (AttachedCam)
					{
						AActor* CamOwner = AttachedCam->GetOwner();
						ImGui::Text("%s", CamOwner ? TCHAR_TO_UTF8(*CamOwner->GetName()) : "Yes");
					}
					else
					{
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "None");
					}

					UStorageCardComponent* EffCard = MicComp->GetEffectiveStorageCard();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Effective Storage Card");
					ImGui::TableSetColumnIndex(1);
					if (EffCard)
					{
						ImGui::Text("%s (ID: %s)", TCHAR_TO_UTF8(*EffCard->CardDisplayName), TCHAR_TO_UTF8(*EffCard->CardID.ToString().Left(8)));
					}
					else
					{
						ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "None (Cannot write)");
					}

					UBatteryConsumerComponent* EffBattery = MicComp->GetEffectiveBatteryConsumer();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Effective Battery");
					ImGui::TableSetColumnIndex(1);
					if (EffBattery)
					{
						if (EffBattery->HasBattery())
						{
							float Pct = EffBattery->GetBatteryChargePercent();
							char ProgressBarBuf[64];
							snprintf(ProgressBarBuf, sizeof(ProgressBarBuf), "%.1f%% (%.1f / %.1f)", Pct, EffBattery->GetBatteryCurrentCharge(), EffBattery->GetBatteryMaxCapacity());
							ImGui::ProgressBar(Pct / 100.f, ImVec2(-FLT_MIN, 0.0f), ProgressBarBuf);
						}
						else
						{
							ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "No Battery slotted in effective power source");
						}
					}
					else
					{
						ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "None (No power)");
					}

					if (OwnerActor)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetAudioMicrophoneNetRoleString(OwnerActor->GetLocalRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Dormancy");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetAudioMicrophoneNetDormancyString(OwnerActor->NetDormancy)));
					}

					ImGui::EndTable();
				}
				ImGui::Unindent();
			}
		}

		if (MicCount == 0)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "No active Audio Microphones found in the level.");
		}

		ImGui::End();
	}
}
