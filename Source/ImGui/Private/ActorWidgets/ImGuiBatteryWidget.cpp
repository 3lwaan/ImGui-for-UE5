#include "ActorWidgets/ImGuiBatteryWidget.h"
#include "BatteryComponent.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <UObject/UObjectIterator.h>

static FString GetBatteryNetRoleString(ENetRole Role)
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

static FString GetBatteryNetDormancyString(ENetDormancy Dormancy)
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

namespace ImGuiBatteryWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Batteries Debug", nullptr, ImGuiWindowFlags_None))
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

		int32 BatteryCount = 0;

		for (TObjectIterator<UBatteryComponent> It; It; ++It)
		{
			UBatteryComponent* BatteryComp = *It;
			if (!IsValid(BatteryComp) || BatteryComp->GetWorld() != World || BatteryComp->IsTemplate())
			{
				continue;
			}

			BatteryCount++;

			AActor* OwnerActor = BatteryComp->GetOwner();
			FString BatteryName = OwnerActor ? OwnerActor->GetName() : BatteryComp->GetName();
			FString BatteryHeader = FString::Printf(TEXT("Battery %d: %s"), BatteryCount, *BatteryName);

			if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*BatteryHeader), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("BatteryProps", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Capacity Tier");
					ImGui::TableSetColumnIndex(1);
					const char* TierStr = "Unknown";
					switch (BatteryComp->Tier)
					{
						case EBatteryTier::Low: TierStr = "Low Capacity"; break;
						case EBatteryTier::Medium: TierStr = "Medium Capacity"; break;
						case EBatteryTier::High: TierStr = "High Capacity"; break;
					}
					ImGui::Text("%s", TierStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Current Charge");
					ImGui::TableSetColumnIndex(1);
					float Pct = BatteryComp->GetChargePercentage();
					char ProgressBarBuf[64];
					snprintf(ProgressBarBuf, sizeof(ProgressBarBuf), "%.1f%% (%.1f / %.1f)", Pct, BatteryComp->CurrentCharge, BatteryComp->MaxCapacity);
					ImGui::ProgressBar(Pct / 100.f, ImVec2(-FLT_MIN, 0.0f), ProgressBarBuf);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Degradation (Wear)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f%%", BatteryComp->Degradation * 100.0f);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Attachment State");
					ImGui::TableSetColumnIndex(1); 
					if (BatteryComp->bIsAttached)
					{
						ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Slotted / Attached");
					}
					else
					{
						ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Freestanding / Unattached");
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Charging State");
					ImGui::TableSetColumnIndex(1);
					if (BatteryComp->bIsCharging)
					{
						ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.8f, 1.0f), "Charging");
					}
					else
					{
						ImGui::Text("Not Charging");
					}

					if (OwnerActor)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetBatteryNetRoleString(OwnerActor->GetLocalRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Dormancy");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetBatteryNetDormancyString(OwnerActor->NetDormancy)));
					}

					ImGui::EndTable();
				}
				ImGui::Unindent();
			}
		}

		if (BatteryCount == 0)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "No active Batteries found in the level.");
		}

		ImGui::End();
	}
}
