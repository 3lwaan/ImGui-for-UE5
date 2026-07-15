#include "ActorWidgets/ImGuiPerformanceWidget.h"

#include <imgui.h>
#include <Engine/Engine.h>
#include <Engine/NetDriver.h>
#include <Engine/NetConnection.h>
#include <Engine/World.h>
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include <Hal/PlatformMemory.h>
#include <Hal/PlatformMisc.h>
#include <Misc/App.h>
#include <DynamicRHI.h>
#include <Modules/ModuleManager.h>
#include <GameFramework/PlayerController.h>
#include <Engine/DebugCameraController.h>
#include <Engine/Player.h>
#include <GameFramework/SpectatorPawn.h>

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

namespace ImGuiPerformanceWidget
{
	void Draw(UWorld* World)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Performance & Optimization Debug", nullptr, ImGuiWindowFlags_None))
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

		// ---------------------------------------------------------------------
		// SECTION 1: System Specs
		// ---------------------------------------------------------------------
		if (ImGui::CollapsingHeader("System Specifications", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();
			if (ImGui::BeginTable("SystemSpecsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("CPU");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*FPlatformMisc::GetCPUBrand()));

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("GPU");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*FPlatformMisc::GetPrimaryGPUBrand()));

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("RHI API");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%s", GDynamicRHI ? TCHAR_TO_UTF8(GDynamicRHI->GetName()) : "Unknown");

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("RAM Status");
				ImGui::TableSetColumnIndex(1);
				FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
				float TotalRAM = MemStats.TotalPhysical / (1024.f * 1024.f * 1024.f);
				float AvailableRAM = MemStats.AvailablePhysical / (1024.f * 1024.f * 1024.f);
				float UsedRAM = TotalRAM - AvailableRAM;
				float PctRAM = TotalRAM > 0.0f ? UsedRAM / TotalRAM : 0.0f;
				char RAMBuf[64];
				snprintf(RAMBuf, sizeof(RAMBuf), "%.1f / %.1f GB Used (%.1f%%)", UsedRAM, TotalRAM, PctRAM * 100.0f);
				ImGui::ProgressBar(PctRAM, ImVec2(-FLT_MIN, 0.0f), RAMBuf);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("OS Version");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*FPlatformMisc::GetOSVersion()));

				if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
				{
					FIntPoint Size = GEngine->GameViewport->Viewport->GetSizeXY();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Resolution");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%dx%d", Size.X, Size.Y);
				}

				ImGui::EndTable();
			}
			ImGui::Unindent();
		}

		// ---------------------------------------------------------------------
		// SECTION 2: Frame Rate & Time Charts
		// ---------------------------------------------------------------------
		if (ImGui::CollapsingHeader("Performance & Timing", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();

			float FrameTimeMs = FApp::GetDeltaTime() * 1000.0f;
			float FPS = ImGui::GetIO().Framerate;

			// Graph logic
			static float FrameTimes[100] = { 0 };
			static int32 FrameTimeIndex = 0;
			FrameTimes[FrameTimeIndex] = FrameTimeMs;
			FrameTimeIndex = (FrameTimeIndex + 1) % 100;

			// Find average / peak
			float AvgTime = 0.0f;
			float PeakTime = 0.0f;
			for (int32 i = 0; i < 100; ++i)
			{
				AvgTime += FrameTimes[i];
				if (FrameTimes[i] > PeakTime)
				{
					PeakTime = FrameTimes[i];
				}
			}
			AvgTime /= 100.0f;

			char GraphLabel[64];
			snprintf(GraphLabel, sizeof(GraphLabel), "Avg: %.1fms, Peak: %.1fms", AvgTime, PeakTime);
			ImGui::Text("Realtime Frame Time Graph:");
			ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 0.8f, 0.8f, 1.0f)); // Bright Cyan
			ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); // Bright Cyan Hovered
			ImGui::PlotLines("##FrameTimeGraph", FrameTimes, 100, FrameTimeIndex, GraphLabel, 0.0f, 60.0f, ImVec2(0.0f, 80.0f));
			ImGui::PopStyleColor(2);

			ImGui::Spacing();

			if (ImGui::BeginTable("TimingMetricsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("FPS (ImGui)");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%.1f FPS", FPS);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text("Delta Time");
				ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f ms", FrameTimeMs);

				ImGui::EndTable();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Viewport Visualization & Utilities");

			// Viewmode dropdown
			static int SelectedViewmode = 0; // Default: Lit
			const char* Viewmodes[] = {
				"Lit",
				"Unlit",
				"Wireframe",
				"Detail Lighting",
				"Lighting Only",
				"Shader Complexity",
				"Light Complexity",
				"Stationary Light Overlap",
				"Quad Overdraw",
				"LOD Coloration",
				"Base Color",
				"Roughness",
				"Normal",
				"Specular",
				"Metallic",
				"Lumen Overview",
				"Lumen Geometry",
				"Lumen Reflections",
				"Lumen Card"
			};

			if (ImGui::Combo("View Mode", &SelectedViewmode, Viewmodes, IM_ARRAYSIZE(Viewmodes)))
			{
				APlayerController* PC = World->GetFirstPlayerController();
				if (PC)
				{
					bool bIsLumen = (SelectedViewmode >= 15);
					if (!bIsLumen)
					{
						PC->ConsoleCommand(TEXT("r.Lumen.Visualize 0"));
					}

					switch (SelectedViewmode)
					{
						case 0:  PC->ConsoleCommand(TEXT("viewmode lit")); break;
						case 1:  PC->ConsoleCommand(TEXT("viewmode unlit")); break;
						case 2:  PC->ConsoleCommand(TEXT("viewmode wireframe")); break;
						case 3:  PC->ConsoleCommand(TEXT("viewmode detaillighting")); break;
						case 4:  PC->ConsoleCommand(TEXT("viewmode lightingonly")); break;
						case 5:  PC->ConsoleCommand(TEXT("viewmode shadercomplexity")); break;
						case 6:  PC->ConsoleCommand(TEXT("viewmode lightcomplexity")); break;
						case 7:  PC->ConsoleCommand(TEXT("viewmode stationarylightoverlap")); break;
						case 8:  PC->ConsoleCommand(TEXT("viewmode quadoverdraw")); break;
						case 9:  PC->ConsoleCommand(TEXT("viewmode lodcoloration")); break;
						case 10: PC->ConsoleCommand(TEXT("viewmode buffervisualization")); PC->ConsoleCommand(TEXT("r.BufferVisualizationTarget BaseColor")); break;
						case 11: PC->ConsoleCommand(TEXT("viewmode buffervisualization")); PC->ConsoleCommand(TEXT("r.BufferVisualizationTarget Roughness")); break;
						case 12: PC->ConsoleCommand(TEXT("viewmode buffervisualization")); PC->ConsoleCommand(TEXT("r.BufferVisualizationTarget WorldNormal")); break;
						case 13: PC->ConsoleCommand(TEXT("viewmode buffervisualization")); PC->ConsoleCommand(TEXT("r.BufferVisualizationTarget Specular")); break;
						case 14: PC->ConsoleCommand(TEXT("viewmode buffervisualization")); PC->ConsoleCommand(TEXT("r.BufferVisualizationTarget Metallic")); break;
						case 15: PC->ConsoleCommand(TEXT("viewmode lit")); PC->ConsoleCommand(TEXT("r.Lumen.Visualize 1")); break;
						case 16: PC->ConsoleCommand(TEXT("viewmode lit")); PC->ConsoleCommand(TEXT("r.Lumen.Visualize 3")); break;
						case 17: PC->ConsoleCommand(TEXT("viewmode lit")); PC->ConsoleCommand(TEXT("r.Lumen.Visualize 4")); break;
						case 18: PC->ConsoleCommand(TEXT("viewmode lit")); PC->ConsoleCommand(TEXT("r.Lumen.Visualize 12")); break;
					}
				}
			}

			// Noclip Debug Camera
			bool bNoclipActive = false;
			for (TObjectIterator<ADebugCameraController> It; It; ++It)
			{
				ADebugCameraController* DCC = *It;
				if (IsValid(DCC) && DCC->GetWorld() == World && !DCC->IsTemplate() && DCC->Player != nullptr)
				{
					bNoclipActive = true;
					break;
				}
			}

			ImGui::Spacing();

			if (bNoclipActive)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red when active
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
				if (ImGui::Button("Disable Noclip (Debug Camera)", ImVec2(-FLT_MIN, 0.0f)))
				{
					ADebugCameraController* DCC = nullptr;
					for (TObjectIterator<ADebugCameraController> It; It; ++It)
					{
						ADebugCameraController* FoundDCC = *It;
						if (IsValid(FoundDCC) && FoundDCC->GetWorld() == World && !FoundDCC->IsTemplate() && FoundDCC->Player != nullptr)
						{
							DCC = FoundDCC;
							break;
						}
					}

					if (DCC && DCC->OriginalPlayer && DCC->OriginalControllerRef)
					{
						DCC->OriginalPlayer->SwitchController(DCC->OriginalControllerRef);
						DCC->OnDeactivate(DCC->OriginalControllerRef);
					}
				}
				ImGui::PopStyleColor(2);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); // Green when inactive
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
				if (ImGui::Button("Enable Noclip (Debug Camera)", ImVec2(-FLT_MIN, 0.0f)))
				{
					APlayerController* PC = World->GetFirstPlayerController();
					if (PC)
					{
						PC->ConsoleCommand(TEXT("ToggleDebugCamera"));
					}
				}
				ImGui::PopStyleColor(2);
			}

			// Freeze Rendering
			static bool bIsRenderingFrozen = false;
			ImGui::Spacing();
			if (bIsRenderingFrozen)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red when frozen
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
				if (ImGui::Button("Unfreeze Rendering", ImVec2(-FLT_MIN, 0.0f)))
				{
					bIsRenderingFrozen = false;
					if (GEngine)
					{
						GEngine->Exec(World, TEXT("FreezeRendering"));
					}
				}
				ImGui::PopStyleColor(2);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); // Green when unfrozen
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
				if (ImGui::Button("Freeze Rendering", ImVec2(-FLT_MIN, 0.0f)))
				{
					bIsRenderingFrozen = true;
					if (GEngine)
					{
						GEngine->Exec(World, TEXT("FreezeRendering"));
					}
				}
				ImGui::PopStyleColor(2);
			}

			ImGui::Unindent();
		}

		// ---------------------------------------------------------------------
		// SECTION 3: Multiplayer Listen-Server Network Info
		// ---------------------------------------------------------------------
		if (ImGui::CollapsingHeader("Multiplayer & Networking", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();

			const char* NetModeStr = "Unknown";
			switch (World->GetNetMode())
			{
				case NM_Client: NetModeStr = "Client"; break;
				case NM_DedicatedServer: NetModeStr = "Dedicated Server"; break;
				case NM_ListenServer: NetModeStr = "Listen Server"; break;
				case NM_Standalone: NetModeStr = "Standalone (Singleplayer)"; break;
			}

			ImGui::Text("Net Mode: %s", NetModeStr);

			UNetDriver* NetDriver = World->GetNetDriver();
			if (NetDriver)
			{
				float InKB = NetDriver->InBytesPerSecond / 1024.f;
				float OutKB = NetDriver->OutBytesPerSecond / 1024.f;

				if (ImGui::BeginTable("NetRatesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Bandwidth (In / Out)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f KBps / %.2f KBps", InKB, OutKB);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); ImGui::Text("Packet Rate (In / Out)");
					ImGui::TableSetColumnIndex(1); ImGui::Text("%u pps / %u pps", NetDriver->InPackets, NetDriver->OutPackets);

					ImGui::EndTable();
				}

				ImGui::Spacing();
				ImGui::Text("Client Network Connections (%d active):", NetDriver->ClientConnections.Num());
				if (NetDriver->ClientConnections.Num() > 0)
				{
					if (ImGui::BeginTable("ConnectionsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
					{
						ImGui::TableSetupColumn("Client Name / IP");
						ImGui::TableSetupColumn("Ping (Latency)");
						ImGui::TableSetupColumn("Loss In");
						ImGui::TableSetupColumn("Loss Out");
						ImGui::TableHeadersRow();

						for (UNetConnection* Conn : NetDriver->ClientConnections)
						{
							if (!IsValid(Conn)) continue;

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::Text("%s", TCHAR_TO_UTF8(*Conn->LowLevelDescribe()));

							// Get Ping if available
							ImGui::TableSetColumnIndex(1);
							float Ping = Conn->PlayerId.IsValid() ? Conn->PlayerId->ToString().Len() : 0.0f; // Dummy fallback
							
							// Find PlayerState matching connection to get real ping
							AGameStateBase* GameState = World->GetGameState();
							FString PlayerName = TEXT("Unknown");
							if (GameState)
							{
								for (APlayerState* PS : GameState->PlayerArray)
								{
									if (PS && PS->GetUniqueId() == Conn->PlayerId)
									{
										PlayerName = PS->GetPlayerName();
										Ping = PS->GetPingInMilliseconds();
										break;
									}
								}
							}
							
							if (PlayerName != TEXT("Unknown"))
							{
								ImGui::Text("%s: %.0f ms", TCHAR_TO_UTF8(*PlayerName), Ping);
							}
							else
							{
								ImGui::Text("%.0f ms", Ping);
							}

							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%.1f%%", Conn->GetInLossPercentage().GetAvgLossPercentage() * 100.0f);

							ImGui::TableSetColumnIndex(3);
							ImGui::Text("%.1f%%", Conn->GetOutLossPercentage().GetAvgLossPercentage() * 100.0f);
						}
						ImGui::EndTable();
					}
				}
				else if (World->GetNetMode() == NM_Client)
				{
					// If client, print server connection stats
					UNetConnection* ServerConn = NetDriver->ServerConnection;
					if (ServerConn)
					{
						ImGui::Text("Connected to Server: %s", TCHAR_TO_UTF8(*ServerConn->LowLevelDescribe()));
						ImGui::Text("Ping to Server: %.0f ms", ServerConn->AvgLag * 1000.0f);
						ImGui::Text("Loss (In / Out): %.1f%% / %.1f%%", 
							ServerConn->GetInLossPercentage().GetAvgLossPercentage() * 100.0f, 
							ServerConn->GetOutLossPercentage().GetAvgLossPercentage() * 100.0f);
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No remote client connections.");
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Network Driver Inactive.");
			}

			ImGui::Unindent();
		}

		// ---------------------------------------------------------------------
		// SECTION 4: Steam Integration Details
		// ---------------------------------------------------------------------
		if (ImGui::CollapsingHeader("Steam Integration Kit", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();

			IOnlineSubsystem* DefaultOSS = IOnlineSubsystem::Get();
			FString DefaultSubName = DefaultOSS ? DefaultOSS->GetSubsystemName().ToString() : TEXT("None");

			ImGui::Text("Default Online Subsystem: %s", TCHAR_TO_UTF8(*DefaultSubName));

			static IOnlineSubsystem* SteamOSS = nullptr;
			static bool bSteamOSSChecked = false;
			if (!bSteamOSSChecked)
			{
				bSteamOSSChecked = true;
				if (FModuleManager::Get().IsModuleLoaded(TEXT("OnlineSubsystemSteam")))
				{
					SteamOSS = IOnlineSubsystem::Get(FName(TEXT("STEAM")));
				}
			}

			if (SteamOSS)
			{
				ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Steam OSS API: Active & Connected");

				IOnlineIdentityPtr Identity = SteamOSS->GetIdentityInterface();
				if (Identity.IsValid())
				{
					TSharedPtr<const FUniqueNetId> NetId = Identity->GetUniquePlayerId(0);
					if (NetId.IsValid())
					{
						ImGui::Text("User SteamID: %s", TCHAR_TO_UTF8(*NetId->ToString()));
						ImGui::Text("Nickname: %s", TCHAR_TO_UTF8(*Identity->GetPlayerNickname(0)));
					}
				}

				IOnlineSessionPtr Session = SteamOSS->GetSessionInterface();
				if (Session.IsValid())
				{
					FNamedOnlineSession* GameSession = Session->GetNamedSession(NAME_GameSession);
					if (GameSession)
					{
						ImGui::Spacing();
						ImGui::Text("Steam Game Session Status:");
						if (ImGui::BeginTable("SteamSessionTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("Session Name");
							ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GameSession->SessionName.ToString()));

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("Players Joined");
							ImGui::TableSetColumnIndex(1); ImGui::Text("%d players", GameSession->RegisteredPlayers.Num());

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("Connections Info");
							int32 TotalSlots = GameSession->SessionSettings.NumPublicConnections + GameSession->SessionSettings.NumPrivateConnections;
							ImGui::TableSetColumnIndex(1); ImGui::Text("%d / %d slots filled", GameSession->RegisteredPlayers.Num(), TotalSlots);

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("Lobby/Session ID");
							if (GameSession->SessionInfo.IsValid())
							{
								ImGui::TableSetColumnIndex(1); ImGui::Text("%s", TCHAR_TO_UTF8(*GameSession->SessionInfo->GetSessionId().ToString()));
							}
							else
							{
								ImGui::TableSetColumnIndex(1); ImGui::Text("Invalid SessionInfo");
							}

							ImGui::EndTable();
						}
					}
					else
					{
						ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No active Steam Game Sessions hosted.");
					}
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "Steam OSS API: Inactive (Standalone / NULL Subsystem)");
			}

			ImGui::Unindent();
		}

		// ---------------------------------------------------------------------
		// SECTION 5: Crosshair Asset Inspector
		// ---------------------------------------------------------------------
		if (ImGui::CollapsingHeader("Crosshair Asset Inspector", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();

			APlayerController* PC = World->GetFirstPlayerController();
			if (PC)
			{
				FVector CameraLocation;
				FRotator CameraRotation;
				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 5000.0f); // 50 meters
				FCollisionQueryParams QueryParams;

				// Ignore local player pawn
				APawn* LocalPawn = PC->GetPawn();
				if (LocalPawn)
				{
					QueryParams.AddIgnoredActor(LocalPawn);
				}
				// Ignore debug camera controller pawn if active
				APawn* SpectatorPawn = PC->GetSpectatorPawn();
				if (SpectatorPawn)
				{
					QueryParams.AddIgnoredActor(SpectatorPawn);
				}

				FHitResult HitResult;
				bool bHit = World->LineTraceSingleByChannel(
					HitResult,
					CameraLocation,
					TraceEnd,
					ECC_Visibility,
					QueryParams
				);

				UStaticMeshComponent* SMC = nullptr;
				if (bHit && HitResult.GetComponent())
				{
					SMC = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				}

				if (SMC && SMC->GetStaticMesh())
				{
					UStaticMesh* Mesh = SMC->GetStaticMesh();
					float DistanceMeters = HitResult.Distance / 100.0f;

					// Estimate LOD index based on screen size (or fallback to 0)
					int32 ActiveLOD = 0;
					if (SMC->GetForcedLodModel() > 0)
					{
						ActiveLOD = SMC->GetForcedLodModel() - 1;
					}
					else
					{
						// A simple distance-based heuristic as a compile-safe fallback for UE 5.6
						if (DistanceMeters > 30.0f && Mesh->GetNumLODs() > 3) ActiveLOD = 3;
						else if (DistanceMeters > 15.0f && Mesh->GetNumLODs() > 2) ActiveLOD = 2;
						else if (DistanceMeters > 5.0f && Mesh->GetNumLODs() > 1) ActiveLOD = 1;
					}

					ImGui::Text("Mesh Name: %s", TCHAR_TO_UTF8(*Mesh->GetName()));
					ImGui::Text("Distance: %.2fm", DistanceMeters);
					ImGui::Text("Active LOD: %d", ActiveLOD);
					ImGui::Text("Total LODs: %d", Mesh->GetNumLODs());

					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Text("Texture Streaming / Mip Telemetry:");

					TArray<UTexture*> Textures;
					SMC->GetUsedTextures(Textures, EMaterialQualityLevel::High);

					TArray<UTexture2D*> Textures2D;
					for (UTexture* Tex : Textures)
					{
						if (UTexture2D* Tex2D = Cast<UTexture2D>(Tex))
						{
							Textures2D.Add(Tex2D);
						}
					}

					if (Textures2D.Num() > 0)
					{
						if (ImGui::BeginTable("TextureTelemetryTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
						{
							ImGui::TableSetupColumn("Texture Name");
							ImGui::TableSetupColumn("Max Resolution");
							ImGui::TableSetupColumn("Active Res (Streaming)");
							ImGui::TableSetupColumn("Mips (Res / Max)");
							ImGui::TableHeadersRow();

							for (UTexture2D* Tex2D : Textures2D)
							{
								if (IsValid(Tex2D))
								{
									int32 BaseWidth = Tex2D->GetSizeX();
									int32 BaseHeight = Tex2D->GetSizeY();
									int32 MaxMips = Tex2D->GetNumMips();
									int32 ResidentMips = Tex2D->GetNumResidentMips();

									int32 ActiveWidth = FMath::Max(1, BaseWidth >> (MaxMips - ResidentMips));
									int32 ActiveHeight = FMath::Max(1, BaseHeight >> (MaxMips - ResidentMips));

									ImGui::TableNextRow();

									// 1. Texture Name
									ImGui::TableSetColumnIndex(0);
									ImGui::Text("%s", TCHAR_TO_UTF8(*Tex2D->GetName()));

									// 2. Max Resolution
									ImGui::TableSetColumnIndex(1);
									ImGui::Text("%dx%d", BaseWidth, BaseHeight);

									// 3. Active Res
									ImGui::TableSetColumnIndex(2);
									if (DistanceMeters > 10.0f && ResidentMips == MaxMips)
									{
										ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.0f, 1.0f), "%dx%d", ActiveWidth, ActiveHeight);
									}
									else
									{
										ImGui::Text("%dx%d", ActiveWidth, ActiveHeight);
									}

									// 4. Mips
									ImGui::TableSetColumnIndex(3);
									ImGui::Text("%d / %d", ResidentMips, MaxMips);
								}
							}

							ImGui::EndTable();
						}
					}
					else
					{
						ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No 2D textures found on mesh materials.");
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Aim at a static mesh to inspect LODs and Mipmaps.");
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "Invalid Player Controller.");
			}

			ImGui::Unindent();
		}

		ImGui::End();
	}
}
