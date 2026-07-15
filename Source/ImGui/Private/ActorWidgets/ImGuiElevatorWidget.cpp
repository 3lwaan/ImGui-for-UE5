#include "ActorWidgets/ImGuiElevatorWidget.h"
#include "ElevatorCoreComponent.h"
#include "ElevatorDoorComponent.h"
#include "ElevatorFloorComponent.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <UObject/UObjectIterator.h>

static FString GetNetRoleString(ENetRole Role)
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

static FString GetNetDormancyString(ENetDormancy Dormancy)
{
	switch (Dormancy)
	{
		case DORM_Never: return TEXT("Never Dormant (Awake)");
		case DORM_Awake: return TEXT("Awake");
		case DORM_DormantAll: return TEXT("Dormant All (Delayed)");
		case DORM_DormantPartial: return TEXT("Dormant Partial");
		case DORM_Initial: return TEXT("Initial Dormant");
		default: return TEXT("Unknown");
	}
}

static FString GetElevatorStateString(EElevatorState State)
{
	switch (State)
	{
		case EElevatorState::Idle: return TEXT("Idle");
		case EElevatorState::DoorsOpening: return TEXT("Doors Opening");
		case EElevatorState::DoorsOpen: return TEXT("Doors Open");
		case EElevatorState::DoorsClosing: return TEXT("Doors Closing");
		case EElevatorState::Moving: return TEXT("Moving");
		case EElevatorState::Arrived: return TEXT("Arrived");
		case EElevatorState::SafetyFault: return TEXT("Safety Fault (E-Stop)");
		default: return TEXT("Unknown");
	}
}

static ImVec4 GetElevatorStateColor(EElevatorState State)
{
	switch (State)
	{
		case EElevatorState::Idle: return ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // Green
		case EElevatorState::DoorsOpening:
		case EElevatorState::DoorsOpen:
		case EElevatorState::DoorsClosing: return ImVec4(0.2f, 0.8f, 0.8f, 1.0f); // Cyan
		case EElevatorState::Moving:
		case EElevatorState::Arrived: return ImVec4(0.9f, 0.8f, 0.2f, 1.0f); // Yellow
		case EElevatorState::SafetyFault: return ImVec4(0.9f, 0.2f, 0.2f, 1.0f); // Red
		default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

static FString GetElevatorDirectionString(EElevatorDirection Direction)
{
	switch (Direction)
	{
		case EElevatorDirection::None: return TEXT("None");
		case EElevatorDirection::Up: return TEXT("Up");
		case EElevatorDirection::Down: return TEXT("Down");
		default: return TEXT("Unknown");
	}
}

static FString GetDoorStateString(EDoorState State)
{
	switch (State)
	{
		case EDoorState::Closed: return TEXT("Closed");
		case EDoorState::Opening: return TEXT("Opening");
		case EDoorState::Open: return TEXT("Open");
		case EDoorState::Closing: return TEXT("Closing");
		case EDoorState::Obstructed: return TEXT("Obstructed (Obstruction)");
		default: return TEXT("Unknown");
	}
}

static ImVec4 GetDoorStateColor(EDoorState State)
{
	switch (State)
	{
		case EDoorState::Closed: return ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Muted grey
		case EDoorState::Opening:
		case EDoorState::Closing: return ImVec4(0.9f, 0.8f, 0.2f, 1.0f); // Yellow
		case EDoorState::Open: return ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // Green
		case EDoorState::Obstructed: return ImVec4(0.9f, 0.2f, 0.2f, 1.0f); // Red
		default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

namespace ImGuiElevatorWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Elevators Diagnostics & Telemetry", nullptr, ImGuiWindowFlags_None))
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

		int32 ElevatorCount = 0;

		for (TObjectIterator<UElevatorCoreComponent> It; It; ++It)
		{
			UElevatorCoreComponent* CoreComp = *It;
			if (!IsValid(CoreComp) || CoreComp->GetWorld() != World || CoreComp->IsTemplate())
			{
				continue;
			}

			ElevatorCount++;

			AActor* OwnerActor = CoreComp->GetOwner();
			FString ActorName = OwnerActor ? OwnerActor->GetName() : CoreComp->GetName();
			FString TagName = CoreComp->ElevatorTag.ToString();
			FString HeaderStr = FString::Printf(TEXT("Elevator %d: Tag [%s] (%s)"), ElevatorCount, *TagName, *ActorName);

			if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*HeaderStr), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("ElevatorProperties", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					// Core State machine
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Elevator State");
					ImGui::TableSetColumnIndex(1);
					FString StateStr = GetElevatorStateString(CoreComp->ElevatorState);
					ImGui::TextColored(GetElevatorStateColor(CoreComp->ElevatorState), "%s", TCHAR_TO_UTF8(*StateStr));

					// Tag
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Elevator Tag");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*TagName));

					// Current Floor Index
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Current Floor");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%d (Display: %s)", CoreComp->CurrentFloorIndex, TCHAR_TO_UTF8(*CoreComp->GetFloorDisplayName(CoreComp->CurrentFloorIndex)));

					// Next Target Floor Index
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Next Target Floor");
					ImGui::TableSetColumnIndex(1);
					int32 TargetFloor = CoreComp->GetTargetFloorIndex();
					ImGui::Text("%d (Display: %s)", TargetFloor, TCHAR_TO_UTF8(*CoreComp->GetFloorDisplayName(TargetFloor)));

					// Travel Direction
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Travel Direction");
					ImGui::TableSetColumnIndex(1);
					FString DirStr = GetElevatorDirectionString(CoreComp->GetTravelDirection());
					ImGui::Text("%s", TCHAR_TO_UTF8(*DirStr));

					// Travel Progress / Progress Bar
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Travel Progress");
					ImGui::TableSetColumnIndex(1);
					float Progress = CoreComp->GetTravelProgress();
					char ProgressBuf[64];
					snprintf(ProgressBuf, sizeof(ProgressBuf), "%.1f%%", Progress * 100.f);
					ImGui::ProgressBar(Progress, ImVec2(-FLT_MIN, 0.0f), ProgressBuf);

					// Travel Speed
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Move Speed (Current / Config)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f cm/s / %.1f cm/s", CoreComp->GetCurrentSpeed(), CoreComp->MoveSpeed);

					// Dwell & Queue Info
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Request Queue Length");
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%d / %d", CoreComp->GetQueueLength(), CoreComp->MaxQueueSize);

					// Queue Elements
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Request Queue");
					ImGui::TableSetColumnIndex(1);
					const TArray<int32>& RequestQueue = CoreComp->GetRequestQueue();
					if (RequestQueue.Num() > 0)
					{
						FString QueueStr;
						for (int32 i = 0; i < RequestQueue.Num(); i++)
						{
							QueueStr += FString::Printf(TEXT("%d"), RequestQueue[i]);
							if (i < RequestQueue.Num() - 1) QueueStr += TEXT(", ");
						}
						ImGui::Text("%s", TCHAR_TO_UTF8(*QueueStr));
					}
					else
					{
						ImGui::Text("Empty");
					}

					// Passengers
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Passengers Count");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%d (Basing updated every frame during movement)", CoreComp->GetPassengerMovementComponents().Num());

					// Replication Details
					if (OwnerActor)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Local Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetNetRoleString(OwnerActor->GetLocalRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Remote Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetNetRoleString(OwnerActor->GetRemoteRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Dormancy");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetNetDormancyString(OwnerActor->NetDormancy)));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Server Z / Client Sim Z");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f / %.1f", CoreComp->RepServerLocationZ, CoreComp->GetClientSimulatedZ());
					}

					// Doors Telemetry
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Inner Door Component");
					ImGui::TableSetColumnIndex(1);
					if (UElevatorDoorComponent* InnerDoor = CoreComp->GetInnerDoor())
					{
						FString InnerDoorStr = GetDoorStateString(InnerDoor->DoorState);
						ImGui::TextColored(GetDoorStateColor(InnerDoor->DoorState), "%s (Open/Close Duration: %.1fs)", TCHAR_TO_UTF8(*InnerDoorStr), InnerDoor->OpenCloseDuration);
					}
					else
					{
						ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "MISSING Inner Door!");
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Active Outer Door Component");
					ImGui::TableSetColumnIndex(1);
					if (UElevatorDoorComponent* OuterDoor = CoreComp->GetActiveOuterDoor())
					{
						FString OuterDoorStr = GetDoorStateString(OuterDoor->DoorState);
						ImGui::TextColored(GetDoorStateColor(OuterDoor->DoorState), "%s (Open/Close Duration: %.1fs)", TCHAR_TO_UTF8(*OuterDoorStr), OuterDoor->OpenCloseDuration);
					}
					else
					{
						ImGui::Text("None / Not Interlocking");
					}

					ImGui::EndTable();
				}

				// Collapsible subsection for registered floor list
				if (ImGui::TreeNode("Registered Floors List"))
				{
					if (ImGui::BeginTable("FloorRegistryTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
					{
						ImGui::TableSetupColumn("Floor Index");
						ImGui::TableSetupColumn("Display Name");
						ImGui::TableSetupColumn("Dock Z Height");
						ImGui::TableSetupColumn("Outer Door State");
						ImGui::TableHeadersRow();

						const TArray<FElevatorFloorEntry>& Floors = CoreComp->GetFloorRegistry();
						for (const FElevatorFloorEntry& Floor : Floors)
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("%d", Floor.FloorIndex);
							ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*Floor.FloorDisplayName));
							ImGui::TableSetColumnIndex(2); ImGui::Text("%.1f", Floor.DockHeight);
							ImGui::TableSetColumnIndex(3);
							if (UElevatorDoorComponent* FloorDoor = Floor.OuterDoor.Get())
							{
								FString FloorDoorStr = GetDoorStateString(FloorDoor->DoorState);
								ImGui::TextColored(GetDoorStateColor(FloorDoor->DoorState), "%s", TCHAR_TO_UTF8(*FloorDoorStr));
							}
							else
							{
								ImGui::Text("No Outer Door Connected");
							}
						}

						ImGui::EndTable();
					}
					ImGui::TreePop();
				}

				ImGui::Unindent();
			}
		}

		if (ElevatorCount == 0)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "No active Elevator Core Components found in the world.");
		}

		ImGui::End();
	}
}
