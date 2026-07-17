#include "ActorWidgets/ActorDebugWidgetsSubsystem.h"
#include "ActorWidgets/ImGuiStorageCardWidget.h"
#include "ActorWidgets/ImGuiVideoCameraWidget.h"
#include "ActorWidgets/ImGuiAudioMicrophoneWidget.h"
#include "ActorWidgets/ImGuiBatteryWidget.h"
#include "ActorWidgets/ImGuiPerformanceWidget.h"
#include "ActorWidgets/ImGuiElevatorWidget.h"
#include "ActorWidgets/ImGuiInsightsTracerWidget.h"
#include "ImGuiDelegates.h"

#include <Engine/World.h>

static TAutoConsoleVariable<int32> CVarDebugStorageCards(
	TEXT("imgui.DebugStorageCards"),
	0,
	TEXT("Toggle the ImGui debug window for Storage Cards.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugVideoCameras(
	TEXT("imgui.DebugVideoCameras"),
	0,
	TEXT("Toggle the ImGui debug window for Video Cameras.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugAudioMicrophones(
	TEXT("imgui.DebugAudioMicrophones"),
	0,
	TEXT("Toggle the ImGui debug window for Audio Microphones.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugBatteries(
	TEXT("imgui.DebugBatteries"),
	0,
	TEXT("Toggle the ImGui debug window for Batteries.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugPerformance(
	TEXT("imgui.DebugPerformance"),
	0,
	TEXT("Toggle the ImGui debug window for Performance & Network Stats.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugElevators(
	TEXT("imgui.DebugElevators"),
	0,
	TEXT("Toggle the ImGui debug window for Elevators.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarDebugInsights(
	TEXT("imgui.DebugInsights"),
	0,
	TEXT("Toggle the ImGui debug window for Unreal Insights Tracer.\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

UActorDebugWidgetsSubsystem::UActorDebugWidgetsSubsystem()
{
}

void UActorDebugWidgetsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FImGuiOutputDevice::Get().Register();

	FImGuiDelegates::OnWorldDebug(GetWorld()).AddUObject(this, &UActorDebugWidgetsSubsystem::DrawDebugWidgets);
}

void UActorDebugWidgetsSubsystem::Deinitialize()
{
	FImGuiDelegates::OnWorldDebug(GetWorld()).RemoveAll(this);

	FImGuiOutputDevice::Get().Unregister();

	Super::Deinitialize();
}

void UActorDebugWidgetsSubsystem::DrawDebugWidgets()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (CVarDebugStorageCards.GetValueOnGameThread() != 0)
	{
		ImGuiStorageCardWidget::Draw(World);
	}

	if (CVarDebugVideoCameras.GetValueOnGameThread() != 0)
	{
		ImGuiVideoCameraWidget::Draw(World);
	}

	if (CVarDebugAudioMicrophones.GetValueOnGameThread() != 0)
	{
		ImGuiAudioMicrophoneWidget::Draw(World);
	}

	if (CVarDebugBatteries.GetValueOnGameThread() != 0)
	{
		ImGuiBatteryWidget::Draw(World);
	}

	if (CVarDebugPerformance.GetValueOnGameThread() != 0)
	{
		ImGuiPerformanceWidget::Draw(World);
	}

	if (CVarDebugElevators.GetValueOnGameThread() != 0)
	{
		ImGuiElevatorWidget::Draw(World);
	}

	if (CVarDebugInsights.GetValueOnGameThread() != 0)
	{
		ImGuiInsightsTracerWidget::Draw(World);
	}
}
