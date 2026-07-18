#include "ActorWidgets/ActorDebugWidgetsSubsystem.h"
#include "ActorWidgets/ImGuiPerformanceWidget.h"
#include "ActorWidgets/ImGuiInsightsTracerWidget.h"
#include "ImGuiDelegates.h"

#include <Engine/World.h>

static TAutoConsoleVariable<int32> CVarDebugPerformance(
	TEXT("imgui.DebugPerformance"),
	0,
	TEXT("Toggle the ImGui debug window for Performance & Network Stats.\n")
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

	if (CVarDebugPerformance.GetValueOnGameThread() != 0)
	{
		ImGuiPerformanceWidget::Draw(World);
	}

	if (CVarDebugInsights.GetValueOnGameThread() != 0)
	{
		ImGuiInsightsTracerWidget::Draw(World);
	}
}
