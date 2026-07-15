#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorDebugWidgetsSubsystem.generated.h"

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

private:
	void DrawDebugWidgets();
};
