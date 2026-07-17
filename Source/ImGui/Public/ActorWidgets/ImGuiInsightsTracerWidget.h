#pragma once

#include "CoreMinimal.h"

#include "Misc/OutputDevice.h"

class UWorld;

struct FLogLine
{
	FString Message;
	ELogVerbosity::Type Verbosity;
	FName Category;
};

class IMGUI_API FImGuiOutputDevice : public FOutputDevice
{
public:
	static FImGuiOutputDevice& Get();

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category) override;

	void Clear();
	TArray<FLogLine> GetLines();
	bool GetAndClearNewLinesFlag();

	void Register();
	void Unregister();

private:
	FImGuiOutputDevice();
	virtual ~FImGuiOutputDevice();

	FCriticalSection CriticalSection;
	TArray<FLogLine> Lines;
	bool bRegistered;
	bool bHasNewLines;
};

namespace ImGuiInsightsTracerWidget
{
	void Draw(UWorld* World);
}
