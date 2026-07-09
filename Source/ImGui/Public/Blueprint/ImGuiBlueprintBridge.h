// ImGuiBlueprintBridge.h
// Production-ready Blueprint Function Library wrapping the core features of Dear ImGui for Unreal Engine.

#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include <UObject/ObjectMacros.h>

#include "ImGuiBlueprintBridge.generated.h"

// ---------------------------------------------------------------------------
// UENUM Flags & Constants (Dear ImGui Wrappers)
// ---------------------------------------------------------------------------

/** ImGuiWindowFlags (Represented as bit indexes for Blueprint uint8 support) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiWindowFlags : uint8
{
	NoTitleBar,
	NoResize,
	NoMove,
	NoScrollbar,
	NoScrollWithMouse,
	NoCollapse,
	AlwaysAutoResize,
	NoBackground,
	NoSavedSettings,
	NoMouseInputs,
	MenuBar,
	HorizontalScrollbar,
	NoFocusOnAppearing,
	NoBringToFrontOnFocus,
	AlwaysVerticalScrollbar,
	AlwaysHorizontalScrollbar,
	AlwaysUseWindowPadding,
	NoNavInputs,
	NoNavFocus,
	UnsavedDocument
};

/** ImGuiInputTextFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiInputTextFlags : uint8
{
	CharsDecimal,
	CharsHexadecimal,
	CharsUppercase,
	CharsNoBlank,
	AutoSelectAll,
	EnterReturnsTrue,
	CallbackCompletion,
	CallbackHistory,
	CallbackAlways,
	CallbackCharFilter,
	AllowTabInput,
	CtrlEnterForNewLine,
	NoHorizontalScroll,
	AlwaysOverwrite,
	ReadOnly,
	Password,
	NoUndoRedo,
	CharsScientific
};

/** ImGuiTreeNodeFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiTreeNodeFlags : uint8
{
	Selected,
	Framed,
	AllowItemOverlap,
	NoTreePushOnOpen,
	NoAutoOpenOnLog,
	DefaultOpen,
	OpenOnDoubleClick,
	OpenOnArrow,
	Leaf,
	Bullet,
	FramePadding,
	SpanAvailWidth,
	SpanFullWidth,
	NavLeftJumpsBackHere
};

/** ImGuiTableFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiTableFlags : uint8
{
	Resizable,
	Reorderable,
	Hideable,
	Sortable,
	NoSavedSettings,
	ContextMenuInBody,
	RowBg,
	BordersInnerH,
	BordersOuterH,
	BordersInnerV,
	BordersOuterV,
	NoBordersInBody,
	NoBordersInBodyUntilResize,
	SizingFixedFit,
	SizingFixedSame,
	SizingStretchProp,
	SizingStretchSame,
	NoHostExtendX,
	NoHostExtendY,
	NoKeepColumnsVisible,
	PreciseWidths,
	NoClip,
	PadOuterX,
	NoPadOuterX,
	NoPadInnerX,
	ScrollX,
	ScrollY,
	SortMulti,
	SortTristate
};

/** ImGuiTableColumnFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiTableColumnFlags : uint8
{
	Disabled,
	DefaultHide,
	DefaultSort,
	WidthStretch,
	WidthFixed,
	NoResize,
	NoReorder,
	NoHide,
	NoClip,
	NoSort,
	NoSortAscending,
	NoSortDescending,
	NoHeaderLabel,
	NoHeaderWidth,
	PreferSortAscending,
	PreferSortDescending,
	IndentEnable,
	IndentDisable
};

/** ImGuiTabBarFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiTabBarFlags : uint8
{
	Reorderable,
	AutoSelectNewTabs,
	TabListPopupButton,
	NoCloseWithMiddleMouseButton,
	NoTabListScrollingButtons,
	NoTooltip,
	FittingPolicyResizeDown,
	FittingPolicyScroll
};

/** ImGuiTabItemFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiTabItemFlags : uint8
{
	UnsavedDocument,
	SetSelected,
	NoCloseWithMiddleMouseButton,
	NoPushId,
	NoTooltip,
	NoReorder,
	Leading,
	Trailing
};

/** ImGuiColorEditFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiColorEditFlags : uint8
{
	NoAlpha,
	NoPicker,
	NoOptions,
	NoSmallPreview,
	NoInputs,
	NoTooltip,
	NoLabel,
	NoSidePreview,
	NoDragDrop,
	NoBorder,
	AlphaBar,
	AlphaPreview,
	AlphaPreviewHalf,
	HDR,
	DisplayRGB,
	DisplayHSV,
	DisplayHex,
	Uint8,
	Float,
	PickerHueBar,
	PickerHueWheel,
	InputRGB,
	InputHSV
};

/** ImGuiSliderFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiSliderFlags : uint8
{
	AlwaysClamp,
	Logarithmic,
	NoRoundToFormat,
	NoInput
};

/** ImGuiPopupFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiPopupFlags : uint8
{
	MouseButtonLeft,
	MouseButtonRight,
	MouseButtonMiddle,
	NoOpenOverExistingPopup,
	NoOpenOverItems,
	AnyPopupId,
	AnyPopupLevel
};

/** ImGuiSelectableFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiSelectableFlags : uint8
{
	DontClosePopups,
	SpanAllColumns,
	AllowDoubleClick,
	Disabled,
	AllowItemOverlap
};

/** ImGuiComboFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiComboFlags : uint8
{
	PopupAlignLeft,
	HeightSmall,
	HeightRegular,
	HeightLarge,
	HeightLargest,
	NoArrowButton,
	NoPreview
};

/** ImGuiHoveredFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiHoveredFlags : uint8
{
	ChildWindows,
	RootWindow,
	AnyWindow,
	NoPopupHierarchy,
	AllowWhenBlockedByPopup,
	AllowWhenBlockedByActiveItem,
	AllowWhenOverlapped,
	AllowWhenDisabled
};

/** ImGuiFocusedFlags (Represented as bit indexes) */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOurImGuiFocusedFlags : uint8
{
	ChildWindows,
	RootWindow,
	AnyWindow,
	NoPopupHierarchy
};

/** Standard ImGuiCol_ styles */
UENUM(BlueprintType)
enum class EOurImGuiCol : uint8
{
	Text,
	TextDisabled,
	WindowBg,
	ChildBg,
	PopupBg,
	Border,
	BorderShadow,
	FrameBg,
	FrameBgHovered,
	FrameBgActive,
	TitleBg,
	TitleBgActive,
	TitleBgCollapsed,
	MenuBarBg,
	ScrollbarBg,
	ScrollbarGrab,
	ScrollbarGrabHovered,
	ScrollbarGrabActive,
	CheckMark,
	SliderGrab,
	SliderGrabActive,
	Button,
	ButtonHovered,
	ButtonActive,
	Header,
	HeaderHovered,
	HeaderActive,
	Separator,
	SeparatorHovered,
	SeparatorActive,
	ResizeGrip,
	ResizeGripHovered,
	ResizeGripActive,
	Tab,
	TabHovered,
	TabActive,
	TabUnfocused,
	TabUnfocusedActive,
	PlotLines,
	PlotLinesHovered,
	PlotHistogram,
	PlotHistogramHovered,
	TableHeaderBg,
	TableBorderStrong,
	TableBorderLight,
	TableRowBg,
	TableRowBgAlt,
	TextSelectedBg,
	DragDropTarget,
	NavHighlight,
	NavWindowingHighlight,
	NavWindowingDimBg,
	ModalWindowDimBg
};

/** Standard ImGuiStyleVar_ styles */
UENUM(BlueprintType)
enum class EOurImGuiStyleVar : uint8
{
	Alpha,
	DisabledAlpha,
	WindowPadding,
	WindowRounding,
	WindowBorderSize,
	WindowMinSize,
	WindowTitleAlign,
	ChildRounding,
	ChildBorderSize,
	PopupRounding,
	PopupBorderSize,
	FramePadding,
	FrameRounding,
	FrameBorderSize,
	ItemSpacing,
	ItemInnerSpacing,
	IndentSpacing,
	CellPadding,
	ScrollbarSize,
	ScrollbarRounding,
	GrabMinSize,
	GrabRounding,
	TabRounding,
	ButtonTextAlign,
	SelectableTextAlign
};

/** Standard ImGui Directions */
UENUM(BlueprintType)
enum class EOurImGuiDir : uint8
{
	None,
	Left,
	Right,
	Up,
	Down
};


// ---------------------------------------------------------------------------
// USTRUCTs for Modular Configurations (Keeping nodes clean via AdvancedDisplay)
// ---------------------------------------------------------------------------

/** Configurations for slider widgets */
USTRUCT(BlueprintType)
struct FImGuiSliderSettings
{
	GENERATED_BODY()

	/** Display format string, e.g. "%.3f" or "Speed: %.1f km/h" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (AdvancedDisplay))
	FString Format = TEXT("%.3f");

	/** Custom ImGui slider flags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiSliderFlags", AdvancedDisplay))
	int32 Flags = 0;
};

/** Configurations for drag-slider widgets */
USTRUCT(BlueprintType)
struct FImGuiDragSettings
{
	GENERATED_BODY()

	/** The speed of change when dragging the slider */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui")
	float Speed = 1.0f;

	/** Display format string, e.g. "%.3f" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (AdvancedDisplay))
	FString Format = TEXT("%.3f");

	/** Custom ImGui slider flags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiSliderFlags", AdvancedDisplay))
	int32 Flags = 0;
};

/** Configurations for input-text widgets */
USTRUCT(BlueprintType)
struct FImGuiInputTextSettings
{
	GENERATED_BODY()

	/** Configuration flags for text editing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiInputTextFlags"))
	int32 Flags = 0;
};

/** Configurations for color editor widgets */
USTRUCT(BlueprintType)
struct FImGuiColorSettings
{
	GENERATED_BODY()

	/** Configuration flags for color picking & editing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImGui", meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiColorEditFlags"))
	int32 Flags = 0;
};


// ---------------------------------------------------------------------------
// UImGuiBlueprintBridge Library
// ---------------------------------------------------------------------------

/**
 * Blueprint Function Library wrapping the core APIs of Dear ImGui.
 * Incorporates safety context validation checks to prevent editor/runtime crashes.
 */
UCLASS()
class UImGuiBlueprintBridge : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// -- Windows & Layout Containers ----------------------------------------

	/**
	 * Begin a standard ImGui window context. Must call EndWindow() at the end of the layout.
	 * Returns true if the window is open and expanded (not collapsed).
	 */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Window", meta = (DisplayName = "Begin Window", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags"))
	static bool BeginWindow(const FString& WindowName, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags")) int32 WindowFlags);

	/**
	 * Begin a standard ImGui window with a close button toggle callback. Must call EndWindow() at the end.
	 * Returns true if the window is open and expanded.
	 */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Window", meta = (DisplayName = "Begin Window with Close Toggle", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags"))
	static bool BeginWindowWithClose(const FString& WindowName, UPARAM(ref) bool& bOpen, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags")) int32 WindowFlags);

	/** Closes the current window scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Window", meta = (DisplayName = "End Window"))
	static void EndWindow();

	/** Begins a scrollable/sub-layout child window scope. Must call EndChild() at the end. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Window", meta = (DisplayName = "Begin Child", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags"))
	static bool BeginChild(const FString& ID, FVector2D Size = FVector2D::ZeroVector, bool bBorder = false, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags")) int32 WindowFlags = 0);

	/** Closes the current child scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Window", meta = (DisplayName = "End Child"))
	static void EndChild();


	// -- Layout Utilities ----------------------------------------------------

	/** Draws a horizontal line separator */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void Separator();

	/** Positions next widget on the same line. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout", meta = (AdvancedDisplay = "OffsetFromStartX, Spacing"))
	static void SameLine(float OffsetFromStartX = 0.0f, float Spacing = -1.0f);

	/** Starts a new line. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void NewLine();

	/** Adds empty space */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void Spacing();

	/** Allocates an invisible dummy widget of specified size */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void Dummy(FVector2D Size);

	/** Indents layout to the right. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void Indent(float IndentWidth = 0.0f);

	/** Unindents layout back to the left. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void Unindent(float IndentWidth = 0.0f);

	/** Locks horizontal layout boundary group. Must call EndGroup(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void BeginGroup();

	/** Closes layout boundary group. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void EndGroup();

	/** Gets local cursor position relative to window start. */
	UFUNCTION(BlueprintPure, Category = "ImGui|Layout")
	static FVector2D GetCursorPos();

	/** Sets local cursor position relative to window start. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void SetCursorPos(FVector2D LocalPos);

	/** Gets screen-space cursor position. */
	UFUNCTION(BlueprintPure, Category = "ImGui|Layout")
	static FVector2D GetCursorScreenPos();

	/** Sets screen-space cursor position. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void SetCursorScreenPos(FVector2D ScreenPos);

	/** Returns available layout space inside the current boundary. */
	UFUNCTION(BlueprintPure, Category = "ImGui|Layout")
	static FVector2D GetContentRegionAvail();

	/** Returns maximum bounds of the content region. */
	UFUNCTION(BlueprintPure, Category = "ImGui|Layout")
	static FVector2D GetContentRegionMax();

	/** Align text baseline to frame padding height. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Layout")
	static void AlignTextToFramePadding();


	// -- ID Stack (ID Conflict Avoidance) -----------------------------------

	/** Push integer ID onto ID collision boundary. Call PopID() at exit. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|IDStack", meta = (DisplayName = "Push ID (Int)"))
	static void PushID_Int(int32 ID);

	/** Push string ID onto ID collision boundary. Call PopID() at exit. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|IDStack", meta = (DisplayName = "Push ID (String)"))
	static void PushID_String(const FString& ID);

	/** Pops top ID boundary off the stack. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|IDStack")
	static void PopID();


	// -- Text widgets --------------------------------------------------------

	/** Renders standard text */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void Text(const FString& TextVal);

	/** Renders custom colored text */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void TextColored(FLinearColor Color, const FString& TextVal);

	/** Renders disabled grayed-out text */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void TextDisabled(const FString& TextVal);

	/** Renders wrapped text automatically fitting current layout width */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void TextWrapped(const FString& TextVal);

	/** Renders a label followed by a value right-aligned */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void LabelText(const FString& Label, const FString& TextVal);

	/** Renders text formatted with a bullet prefix */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Text")
	static void BulletText(const FString& TextVal);


	// -- Buttons & Inputs ---------------------------------------------------

	/**
	 * Render a clickable button. Exposes a direct execution flow branch
	 * for True/False out of the node to reduce visual layout spaghetti.
	 */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets", meta = (DisplayName = "Button", ExpandBoolAsExecs = "ReturnValue"))
	static bool DebugButton(const FString& Label, FVector2D Size = FVector2D::ZeroVector);

	/** Render a small tight fit button. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SmallButton(const FString& Label);

	/** Render an invisible button that catches hit inputs. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool InvisibleButton(const FString& ID, FVector2D Size);

	/** Render a directional arrow button. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool ArrowButton(const FString& ID, EOurImGuiDir Dir);

	/** Checkbox returning updated value to variable. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets")
	static void Checkbox(const FString& Label, bool CurrentValue, bool& OutValue);

	/** Renders a classic circular radio selection button. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets")
	static bool RadioButton(const FString& Label, bool bActive);

	/** Radio Button with automated index state updates. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets")
	static void RadioButtonState(const FString& Label, int32 CurrentValue, int32 ButtonValue, int32& OutValue);

	/** Renders a loading or progress status bar */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets", meta = (AdvancedDisplay = "Size, Overlay"))
	static void ProgressBar(float Fraction, FVector2D Size = FVector2D(0.0f, 0.0f), const FString& Overlay = TEXT(""));

	/** Draws a simple dot bullet */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets")
	static void Bullet();


	// -- Float & Int Sliders (Pass-by-Reference Mutation) -------------------

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderFloat(const FString& Label, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderFloat2(const FString& Label, FVector2D CurrentValue, float Min, float Max, FVector2D& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderFloat3(const FString& Label, FVector CurrentValue, float Min, float Max, FVector& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderFloat4(const FString& Label, FVector4 CurrentValue, float Min, float Max, FVector4& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderInt(const FString& Label, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void SliderInt2(const FString& Label, int32 CurrentValX, int32 CurrentValY, int32 Min, int32 Max, int32& OutValX, int32& OutValY, const FImGuiSliderSettings& Settings);

	/** Vertical sliders */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void VSliderFloat(const FString& Label, FVector2D Size, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiSliderSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Sliders", meta = (AdvancedDisplay = "Settings"))
	static void VSliderInt(const FString& Label, FVector2D Size, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiSliderSettings& Settings);


	// -- Drag Adjusters (Pass-by-Reference Mutation) -----------------------

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragFloat(const FString& Label, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiDragSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragFloat2(const FString& Label, FVector2D CurrentValue, float Min, float Max, FVector2D& OutValue, const FImGuiDragSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragFloat3(const FString& Label, FVector CurrentValue, float Min, float Max, FVector& OutValue, const FImGuiDragSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragFloat4(const FString& Label, FVector4 CurrentValue, float Min, float Max, FVector4& OutValue, const FImGuiDragSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragInt(const FString& Label, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiDragSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Drags", meta = (AdvancedDisplay = "Settings"))
	static void DragInt2(const FString& Label, int32 CurrentValX, int32 CurrentValY, int32 Min, int32 Max, int32& OutValX, int32& OutValY, const FImGuiDragSettings& Settings);


	// -- TextInput Keyboard Fields ------------------------------------------

	/** Editable text input string field */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Settings"))
	static void InputText(const FString& Label, const FString& CurrentValue, FString& OutValue, const FImGuiInputTextSettings& Settings);

	/** Multi-line edit text field */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Settings"))
	static void InputTextMultiline(const FString& Label, const FString& CurrentValue, FString& OutValue, FVector2D Size, const FImGuiInputTextSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Step, StepFast, Settings"))
	static void InputFloat(const FString& Label, float CurrentValue, float Step, float StepFast, float& OutValue, const FImGuiInputTextSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Settings"))
	static void InputFloat2(const FString& Label, FVector2D CurrentValue, FVector2D& OutValue, const FImGuiInputTextSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Settings"))
	static void InputFloat3(const FString& Label, FVector CurrentValue, FVector& OutValue, const FImGuiInputTextSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Settings"))
	static void InputFloat4(const FString& Label, FVector4 CurrentValue, FVector4& OutValue, const FImGuiInputTextSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Inputs", meta = (AdvancedDisplay = "Step, StepFast, Settings"))
	static void InputInt(const FString& Label, int32 CurrentValue, int32 Step, int32 StepFast, int32& OutValue, const FImGuiInputTextSettings& Settings);


	// -- Color Selectors ----------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "ImGui|Color", meta = (AdvancedDisplay = "Settings"))
	static void ColorEdit3(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Color", meta = (AdvancedDisplay = "Settings"))
	static void ColorEdit4(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Color", meta = (AdvancedDisplay = "Settings"))
	static void ColorPicker3(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Color", meta = (AdvancedDisplay = "Settings"))
	static void ColorPicker4(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Color", meta = (AdvancedDisplay = "Flags, Size"))
	static bool ColorButton(const FString& ID, FLinearColor Color, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiColorEditFlags")) int32 Flags = 0, FVector2D Size = FVector2D::ZeroVector);


	// -- Combos & ListBoxes -------------------------------------------------

	/** Scope start combo dropdown. Must call EndCombo(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos", meta = (DisplayName = "Begin Combo", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiComboFlags"))
	static bool BeginCombo(const FString& Label, const FString& PreviewValue, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiComboFlags")) int32 Flags = 0);

	/** End current combo context. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos", meta = (DisplayName = "End Combo"))
	static void EndCombo();

	/** Complete combo dropdown widget helper. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos")
	static void Combo(const FString& Label, int32 CurrentIndex, const TArray<FString>& Items, int32& OutIndex, int32 HeightInItems = -1);

	/** Scope listbox. Must call EndListBox(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos", meta = (DisplayName = "Begin ListBox"))
	static bool BeginListBox(const FString& Label, FVector2D Size = FVector2D::ZeroVector);

	/** End listbox scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos", meta = (DisplayName = "End ListBox"))
	static void EndListBox();

	/** Complete listbox selection selector. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Combos")
	static void ListBox(const FString& Label, int32 CurrentIndex, const TArray<FString>& Items, int32& OutIndex, int32 HeightInItems = -1);


	// -- Tree Nodes & Selectables -------------------------------------------

	/** Collapsible tree node. Must call TreePop() if returns true. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Trees", meta = (DisplayName = "Tree Node", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags"))
	static bool TreeNode(const FString& Label, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags")) int32 Flags = 0);

	/** Exit tree node block. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Trees", meta = (DisplayName = "Tree Pop"))
	static void TreePop();

	/** Collapsing header. Returns true if expanded. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Trees", meta = (DisplayName = "Collapsing Header", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags"))
	static bool CollapsingHeader(const FString& Label, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags")) int32 Flags = 0);

	/** Collapsing header with a visible/open close toggle. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Trees", meta = (DisplayName = "Collapsing Header with Close", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags"))
	static bool CollapsingHeaderWithClose(const FString& Label, UPARAM(ref) bool& bVisible, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTreeNodeFlags")) int32 Flags = 0);

	/** Render a selectable label item. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Trees", meta = (DisplayName = "Selectable", AdvancedDisplay = "Flags, Size"))
	static bool Selectable(const FString& Label, bool bSelected, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiSelectableFlags")) int32 Flags = 0, FVector2D Size = FVector2D::ZeroVector);


	// -- Tooltips -----------------------------------------------------------

	/** Begins a custom tooltip window layout scope. Must call EndTooltip() at exit. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips", meta = (DisplayName = "Begin Tooltip"))
	static void BeginTooltip();

	/** Ends tooltip scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips", meta = (DisplayName = "End Tooltip"))
	static void EndTooltip();

	/** Simple text hover tooltip helper. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips", meta = (DisplayName = "Set Tooltip"))
	static void SetTooltip(const FString& TextVal);


	// -- Popups & Modals ----------------------------------------------------

	/** Begins popup scope. Must call EndPopup(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups", meta = (DisplayName = "Begin Popup", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags"))
	static bool BeginPopup(const FString& ID, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags")) int32 WindowFlags = 0);

	/** Begins popup modal scope. Must call EndPopup(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups", meta = (DisplayName = "Begin Popup Modal", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags"))
	static bool BeginPopupModal(const FString& Name, UPARAM(ref) bool& bOpen, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiWindowFlags")) int32 WindowFlags = 0);

	/** End current popup window context. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups", meta = (DisplayName = "End Popup"))
	static void EndPopup();

	/** Trigger a popup display. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups", meta = (DisplayName = "Open Popup", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiPopupFlags"))
	static void OpenPopup(const FString& ID, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiPopupFlags")) int32 PopupFlags = 0);

	/** Closes active popup scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups", meta = (DisplayName = "Close Current Popup"))
	static void CloseCurrentPopup();

	/** Query popup state. */
	UFUNCTION(BlueprintPure, Category = "ImGui|Popups", meta = (DisplayName = "Is Popup Open", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiPopupFlags"))
	static bool IsPopupOpen(const FString& ID, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiPopupFlags")) int32 PopupFlags = 0);


	// -- Style Push / Pop Stack ---------------------------------------------

	/** Overrides a style color element value. Pair with PopStyleColor(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style", meta = (DisplayName = "Push Style Color"))
	static void PushStyleColor(EOurImGuiCol Element, FLinearColor Color);

	/** Pops overridden style color entries. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style", meta = (DisplayName = "Pop Style Color"))
	static void PopStyleColor(int32 Count = 1);

	/** Push float style variable override. Pair with PopStyleVar(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style", meta = (DisplayName = "Push Style Var (Float)"))
	static void PushStyleVarFloat(EOurImGuiStyleVar Var, float Value);

	/** Push Vec2 style variable override. Pair with PopStyleVar(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style", meta = (DisplayName = "Push Style Var (Vector2D)"))
	static void PushStyleVarVec2(EOurImGuiStyleVar Var, FVector2D Value);

	/** Pops style variable overrides. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style", meta = (DisplayName = "Pop Style Var"))
	static void PopStyleVar(int32 Count = 1);


	// -- Tables (Modern Grid Systems) ---------------------------------------

	/** Grid container. Must call EndTable() if returns true. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "Begin Table", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTableFlags", AdvancedDisplay = "OuterSize, InnerWidth"))
	static bool BeginTable(const FString& ID, int32 ColumnsCount, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTableFlags")) int32 Flags = 0, FVector2D OuterSize = FVector2D::ZeroVector, float InnerWidth = 0.0f);

	/** Close grid context. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "End Table"))
	static void EndTable();

	/** Move layout focus onto next column slot. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "Table Next Column"))
	static void TableNextColumn();

	/** Start a new table row. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "Table Next Row", AdvancedDisplay = "RowFlags, MinRowHeight"))
	static void TableNextRow(int32 RowFlags = 0, float MinRowHeight = 0.0f);

	/** Setup layout properties of individual table column index. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "Table Setup Column", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTableColumnFlags", AdvancedDisplay = "InitWidthOrWeight, UserID"))
	static void TableSetupColumn(const FString& Label, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTableColumnFlags")) int32 Flags = 0, float InitWidthOrWeight = 0.0f, int32 UserID = 0);

	/** Render generated column setup headers in a row. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables", meta = (DisplayName = "Table Headers Row"))
	static void TableHeadersRow();


	// -- Tab Bars & Items ----------------------------------------------------

	/** Begins a tab collection. Must call EndTabBar() if true. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tabs", meta = (DisplayName = "Begin Tab Bar", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTabBarFlags"))
	static bool BeginTabBar(const FString& ID, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTabBarFlags")) int32 Flags = 0);

	/** Ends tab bar collection. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tabs", meta = (DisplayName = "End Tab Bar"))
	static void EndTabBar();

	/** Begins single tab item within a bar. Must call EndTabItem() if true. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tabs", meta = (DisplayName = "Begin Tab Item", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTabItemFlags"))
	static bool BeginTabItem(const FString& Label, UPARAM(ref) bool& bOpen, UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiTabItemFlags")) int32 Flags = 0);

	/** Ends tab item scope. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tabs", meta = (DisplayName = "End Tab Item"))
	static void EndTabItem();

	/** Focuses close state of tab. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tabs", meta = (DisplayName = "Set Tab Item Closed"))
	static void SetTabItemClosed(const FString& TabOrBarLabel);


	// -- Columns (Old Layout API) -------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "ImGui|Columns")
	static void Columns(int32 ColumnsCount = 1, const FString& ID = TEXT(""), bool bBorder = true);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Columns")
	static void NextColumn();


	// -- Item State Queries -------------------------------------------------

	/** Checks if the last rendered widget item is hovered. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries", meta = (DisplayName = "Is Item Hovered", Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiHoveredFlags"))
	static bool IsItemHovered(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ImGui.EOurImGuiHoveredFlags")) int32 Flags = 0);

	/** Checks if the last item is active. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static bool IsItemActive();

	/** Checks if last item is focused. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static bool IsItemFocused();

	/** Checks if last item is clicked. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static bool IsItemClicked(int32 MouseButton = 0);

	/** Checks if last item was edited/modified. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static bool IsItemEdited();

	/** Checks if last item value just deactivated after edit. */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static bool IsItemDeactivatedAfterEdit();

	/** Gets minimum vertex point of the last item rect */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static FVector2D GetItemRectMin();

	/** Gets maximum vertex point of the last item rect */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static FVector2D GetItemRectMax();

	/** Gets size of last item bounding box */
	UFUNCTION(BlueprintPure, Category = "ImGui|ItemQueries")
	static FVector2D GetItemRectSize();


	// -- Demo & Debug Windows -----------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo")
	static void ShowDemoWindow(UPARAM(ref) bool& bOpen);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo")
	static void ShowMetricsWindow(UPARAM(ref) bool& bOpen);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo")
	static void ShowAboutWindow(UPARAM(ref) bool& bOpen);

	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo")
	static void ShowUserGuide();


private:

	/** Convert color elements enum value to raw ImGuiCol_ values */
	static int32 ResolveColorIndex(EOurImGuiCol Element);

	/** Convert style var enum values to raw ImGuiStyleVar_ values */
	static int32 ResolveStyleVarIndex(EOurImGuiStyleVar Var);
};
