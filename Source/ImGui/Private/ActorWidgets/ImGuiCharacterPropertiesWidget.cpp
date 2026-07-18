#include "ActorWidgets/ImGuiCharacterPropertiesWidget.h"

#include <imgui.h>
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <UObject/WeakObjectPtr.h>

namespace ImGuiCharacterPropertiesWidget
{
	// Track states across frames
	static bool bShowCollision = false;
	static bool bShowBones = false;
	static bool bVisualizeMovement = false;
	static bool bDebugTrajectory = false;
	static bool bDebugMMQuery = false;
	static bool bDebugRootOffset = false;
	static bool bDebugOrientationWarping = false;
	static bool bDebugFootPlacement = false;

	static TWeakObjectPtr<UUserWidget> ControllerWidgetInstance = nullptr;

	static void AddTooltip(const char* Text)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(Text);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Draw(UWorld* World)
	{
		if (!World)
		{
			return;
		}

		ImGui::SetNextWindowSize(ImVec2(550, 400), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Character Properties Debugger", nullptr, ImGuiWindowFlags_None))
		{
			ImGui::End();
			return;
		}

		APlayerController* PC = World->GetFirstPlayerController();
		if (!PC)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: No Active Player Controller found!");
			ImGui::End();
			return;
		}

		// Header title with sleek theme color
		ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.8f, 1.0f), "Character Diagnostics & Telemetry");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginTabBar("CharacterDebugTabBar", ImGuiTabBarFlags_None))
		{
			// =================================================================
			// TAB 1: Built-in Engine Tools
			// =================================================================
			if (ImGui::BeginTabItem("Built-in Diagnostics"))
			{
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.6f, 0.8f, 0.6f, 1.0f), "[Engine Built-in Features]");
				ImGui::TextWrapped("The following options are natively supported by Unreal Engine and will function in any project configuration.");
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				// Collision
				if (ImGui::Checkbox("Show Collision Volumes", &bShowCollision))
				{
					PC->ConsoleCommand(TEXT("Show Collision"));
				}
				AddTooltip("Toggle collision shape rendering for all actors (Console command: 'Show Collision')");

				ImGui::Spacing();

				// Bones
				if (ImGui::Checkbox("Show Skeletal Bones", &bShowBones))
				{
					PC->ConsoleCommand(TEXT("Show Bones"));
				}
				AddTooltip("Toggle skeletal mesh skeleton rendering (Console command: 'Show Bones')");

				ImGui::Spacing();

				// Movement
				if (ImGui::Checkbox("Visualize Character Movement", &bVisualizeMovement))
				{
					PC->ConsoleCommand(bVisualizeMovement ? TEXT("p.VisualizeMovement 1") : TEXT("p.VisualizeMovement 0"));
				}
				AddTooltip("Visualize paths, target velocity, and floor collision checks (Console command: 'p.VisualizeMovement')");

				ImGui::EndTabItem();
			}

			// =================================================================
			// TAB 2: Advanced Animation / Motion Matching
			// =================================================================
			if (ImGui::BeginTabItem("Anim & Motion Matching"))
			{
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "[Requires Animation Plugins]");
				ImGui::TextWrapped("The diagnostics below require the 'Motion Matching' and 'Animation Warping' plugins to be enabled in your project settings.");
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				// Trajectory
				if (ImGui::Checkbox("Draw Trajectory History", &bDebugTrajectory))
				{
					PC->ConsoleCommand(bDebugTrajectory ? TEXT("a.AnimNode.PoseHistory.DebugDrawTrajectory 1") : TEXT("a.AnimNode.PoseHistory.DebugDrawTrajectory 0"));
				}
				AddTooltip("Draw past and predicted future character trajectory (Console command: 'a.AnimNode.PoseHistory.DebugDrawTrajectory')");

				ImGui::Spacing();

				// Motion Matching Query
				if (ImGui::Checkbox("Draw Motion Matching Query", &bDebugMMQuery))
				{
					PC->ConsoleCommand(bDebugMMQuery ? TEXT("a.AnimNode.MotionMatching.DebugDrawQuery 1") : TEXT("a.AnimNode.MotionMatching.DebugDrawQuery 0"));
				}
				AddTooltip("Draw query vectors against the motion matching database (Console command: 'a.AnimNode.MotionMatching.DebugDrawQuery')");

				ImGui::Spacing();

				// Root Offset
				if (ImGui::Checkbox("Debug Root Bone Offset", &bDebugRootOffset))
				{
					PC->ConsoleCommand(bDebugRootOffset ? TEXT("a.AnimNode.OffsetRootBone.Debug 1") : TEXT("a.AnimNode.OffsetRootBone.Debug 0"));
				}
				AddTooltip("Draw root bone offset telemetry (Console command: 'a.AnimNode.OffsetRootBone.Debug')");

				ImGui::Spacing();

				// Orientation Warping
				if (ImGui::Checkbox("Debug Orientation Warping", &bDebugOrientationWarping))
				{
					PC->ConsoleCommand(bDebugOrientationWarping ? TEXT("a.AnimNode.OrientationWarping.Debug 1") : TEXT("a.AnimNode.OrientationWarping.Debug 0"));
				}
				AddTooltip("Draw orientation warping angle correction values (Console command: 'a.AnimNode.OrientationWarping.Debug')");

				ImGui::Spacing();

				// Foot Placement
				if (ImGui::Checkbox("Debug Foot Placement IK", &bDebugFootPlacement))
				{
					PC->ConsoleCommand(bDebugFootPlacement ? TEXT("a.AnimNode.FootPlacement.Debug 1") : TEXT("a.AnimNode.FootPlacement.Debug 0"));
				}
				AddTooltip("Draw foot placement IK trace lines and target offsets (Console command: 'a.AnimNode.FootPlacement.Debug')");

				ImGui::EndTabItem();
			}

			// =================================================================
			// TAB 3: Controller UI Widget Utilities
			// =================================================================
			if (ImGui::BeginTabItem("UI Overlays"))
			{
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.8f, 1.0f), "[Controller Widget Utility]");
				ImGui::TextWrapped("Toggle the standard controller overlay (WB_Controller) directly from the plugin's content directory.");
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				bool bIsVisible = ControllerWidgetInstance.IsValid() && ControllerWidgetInstance->IsInViewport();

				ImGui::Text("Overlay Status: ");
				ImGui::SameLine();
				if (bIsVisible)
				{
					ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "VISIBLE IN VIEWPORT");
				}
				else
				{
					ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "HIDDEN");
				}

				ImGui::Spacing();

				if (bIsVisible)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
					if (ImGui::Button("Hide Controller Overlay", ImVec2(-FLT_MIN, 35.0f)))
					{
						ControllerWidgetInstance->RemoveFromParent();
						ControllerWidgetInstance = nullptr;
					}
					ImGui::PopStyleColor(2);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
					if (ImGui::Button("Show Controller Overlay", ImVec2(-FLT_MIN, 35.0f)))
					{
						UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/ImGui/Widgets/WB_Controller.WB_Controller_C"));
						if (WidgetClass)
						{
							UUserWidget* NewWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
							if (NewWidget)
							{
								NewWidget->AddToViewport();
								ControllerWidgetInstance = NewWidget;
							}
						}
					}
					ImGui::PopStyleColor(2);
					AddTooltip("Spawn and add the WB_Controller overlay to the player viewport.");
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}
