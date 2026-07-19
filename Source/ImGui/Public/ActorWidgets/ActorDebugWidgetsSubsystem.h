#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorDebugWidgetsSubsystem.generated.h"

class UUserWidget;

/**
 * World subsystem that registers ImGui debug draw callbacks to present
 * in-game telemetry/diagnostics for level actors.
 */
UCLASS()
class IMGUI_API UActorDebugWidgetsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UActorDebugWidgetsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Per-world debug toggles to isolate state across multiple local players/clients in PIE
	bool bShowCollision = false;
	bool bShowBones = false;
	bool bVisualizeMovement = false;
	bool bDebugTrajectory = false;
	bool bDebugMMQuery = false;
	bool bDebugRootOffset = false;
	bool bDebugOrientationWarping = false;
	bool bDebugFootPlacement = false;

	UPROPERTY()
	TWeakObjectPtr<UUserWidget> ControllerWidgetInstance = nullptr;

	// Performance widget states
	float FrameTimes[100] = { 0.0f };
	int32 FrameTimeIndex = 0;
	int32 SelectedViewmode = 0;
	bool bIsRenderingFrozen = false;

	// Insights widget states
	bool bIsTracing = false;
	float TraceThroughput[100] = { 0.0f };
	int32 TraceThroughputIndex = 0;
	bool bShowOutputLog = false;

private:
	void DrawDebugWidgets();
};
