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
			ImGui::PlotLines("##FrameTimeGraph", FrameTimes, 100, FrameTimeIndex, GraphLabel, 0.0f, 60.0f, ImVec2(-FLT_MIN, 80));

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

			IOnlineSubsystem* SteamOSS = IOnlineSubsystem::Get(FName(TEXT("STEAM")));
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

		ImGui::End();
	}
}
