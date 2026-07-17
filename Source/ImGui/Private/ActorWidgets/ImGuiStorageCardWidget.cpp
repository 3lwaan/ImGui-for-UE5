#include "ActorWidgets/ImGuiStorageCardWidget.h"
#include "StorageCardComponent.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <UObject/UObjectIterator.h>

static FString GetStorageCardNetRoleString(ENetRole Role)
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

static FString GetStorageCardNetDormancyString(ENetDormancy Dormancy)
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

namespace ImGuiStorageCardWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Storage Cards Debug", nullptr, ImGuiWindowFlags_None))
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

		int32 CardCount = 0;

		for (TObjectIterator<UStorageCardComponent> It; It; ++It)
		{
			UStorageCardComponent* CardComp = *It;
			if (!IsValid(CardComp) || CardComp->GetWorld() != World || CardComp->IsTemplate())
			{
				continue;
			}

			CardCount++;

			FString CardHeader = FString::Printf(TEXT("Card %d: %s (ID: %s)"),
				CardCount,
				*CardComp->CardDisplayName,
				*CardComp->CardID.ToString().Left(8));

			if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*CardHeader), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("CardProps", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Serial Number");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*CardComp->CardSerialNumber));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("State");
					ImGui::TableSetColumnIndex(1);
					EStorageCardState State = CardComp->CardState;
					const char* StateStr = "Unknown";
					ImVec4 StateColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
					switch (State)
					{
						case EStorageCardState::Idle: StateStr = "Idle"; StateColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); break;
						case EStorageCardState::InsertedInCamera: StateStr = "In Camera"; StateColor = ImVec4(0.2f, 0.8f, 0.2f, 1.0f); break;
						case EStorageCardState::InsertedInMicrophone: StateStr = "In Microphone"; StateColor = ImVec4(0.2f, 0.6f, 0.8f, 1.0f); break;
						case EStorageCardState::InsertedInComputer: StateStr = "In Computer"; StateColor = ImVec4(0.8f, 0.5f, 0.2f, 1.0f); break;
						case EStorageCardState::Recording: StateStr = "Recording"; StateColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f); break;
					}
					ImGui::TextColored(StateColor, "%s", StateStr);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Capacity");
					ImGui::TableSetColumnIndex(1);
					float Used = CardComp->UsedCapacityMB;
					float Total = CardComp->TotalCapacityMB;
					float Pct = Total > 0.0f ? Used / Total : 0.0f;
					char Buf[64];
					snprintf(Buf, sizeof(Buf), "%.1f / %.1f MB (%.1f%%)", Used, Total, Pct * 100.0f);
					ImGui::ProgressBar(Pct, ImVec2(-FLT_MIN, 0.0f), Buf);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Speeds (Write/Read)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f MB/s / %.1f MB/s", CardComp->WriteSpeedMBps, CardComp->ReadSpeedMBps);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Write Protected");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%s", CardComp->bIsWriteProtected ? "Yes" : "No");

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Session Timeout");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f seconds", CardComp->SessionTimeoutSeconds);

					AActor* OwnerActor = CardComp->GetOwner();
					if (OwnerActor)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Role");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetStorageCardNetRoleString(OwnerActor->GetLocalRole())));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Network Dormancy");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GetStorageCardNetDormancyString(OwnerActor->NetDormancy)));
					}

					ImGui::EndTable();
				}

				ImGui::Spacing();
				ImGui::Text("Active Session Details:");
				if (CardComp->IsRecordingSessionActive())
				{
					if (ImGui::BeginTable("SessionProps", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Session ID");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*CardComp->GetActiveSessionID().ToString()));

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Writer Player ID");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%d", CardComp->GetSessionWriterPlayerId());

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text("Pending Finalizations (V3)");
						ImGui::TableSetColumnIndex(1); ImGui::Text("%d", CardComp->GetPendingFinalizations());

						ImGui::EndTable();
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No Active Session");
				}

				ImGui::Spacing();
				ImGui::Text("File Directory (%d files):", CardComp->GetFileCount());
				TArray<FStorageFileEntry> Files = CardComp->GetAllFiles();
				if (Files.Num() > 0)
				{
					if (ImGui::BeginTable("FilesTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
					{
						ImGui::TableSetupColumn("Name");
						ImGui::TableSetupColumn("Type");
						ImGui::TableSetupColumn("Size");
						ImGui::TableSetupColumn("Duration");
						ImGui::TableSetupColumn("Details / Payload");
						ImGui::TableHeadersRow();

						for (const FStorageFileEntry& File : Files)
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							if (File.bIsCorrupted)
							{
								ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "%s [CORRUPT]", TCHAR_TO_UTF8(*File.FileName));
							}
							else
							{
								ImGui::Text("%s", TCHAR_TO_UTF8(*File.FileName));
							}

							ImGui::TableSetColumnIndex(1);
							const char* TypeStr = "Unknown";
							switch (File.FileType)
							{
								case EStorageFileType::VideoDescriptor: TypeStr = "Video"; break;
								case EStorageFileType::AudioDescriptor: TypeStr = "Audio"; break;
								case EStorageFileType::PhotoCapture: TypeStr = "Photo"; break;
								case EStorageFileType::ExportedFilm: TypeStr = "Exported Film"; break;
							}
							ImGui::Text("%s", TypeStr);

							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%.2f MB", File.FileSizeMB);

							ImGui::TableSetColumnIndex(3);
							ImGui::Text("%.2f s", File.DurationSeconds);

							ImGui::TableSetColumnIndex(4);
							FString ButtonLabel = FString::Printf(TEXT("Show Payload##%s"), *File.FileID.ToString());
							if (ImGui::TreeNode(TCHAR_TO_UTF8(*ButtonLabel)))
							{
								ImGui::TextWrapped("ID: %s", TCHAR_TO_UTF8(*File.FileID.ToString()));
								ImGui::TextWrapped("Created: %s", TCHAR_TO_UTF8(*File.CreationTime.ToString()));
								ImGui::TextWrapped("Payload: %s", TCHAR_TO_UTF8(*File.DescriptorPayload));
								ImGui::TreePop();
							}
						}
						ImGui::EndTable();
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Card is empty");
				}

				ImGui::Unindent();
			}
		}

		if (CardCount == 0)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "No active Storage Cards found in the level.");
		}

		ImGui::End();
	}
}
