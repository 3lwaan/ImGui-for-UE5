// ImGuiBlueprintBridge.cpp
// Implementation of the ImGui Blueprint Function Library wrapper.

#include "Blueprint/ImGuiBlueprintBridge.h"

#include <imgui.h>
#include <string>

// ---------------------------------------------------------------------------
// Conversion Utilities
// ---------------------------------------------------------------------------

int32 UImGuiBlueprintBridge::ResolveColorIndex(EOurImGuiCol Element)
{
	switch (Element)
	{
	case EOurImGuiCol::Text:                  return ImGuiCol_Text;
	case EOurImGuiCol::TextDisabled:          return ImGuiCol_TextDisabled;
	case EOurImGuiCol::WindowBg:              return ImGuiCol_WindowBg;
	case EOurImGuiCol::ChildBg:               return ImGuiCol_ChildBg;
	case EOurImGuiCol::PopupBg:               return ImGuiCol_PopupBg;
	case EOurImGuiCol::Border:                return ImGuiCol_Border;
	case EOurImGuiCol::BorderShadow:          return ImGuiCol_BorderShadow;
	case EOurImGuiCol::FrameBg:               return ImGuiCol_FrameBg;
	case EOurImGuiCol::FrameBgHovered:        return ImGuiCol_FrameBgHovered;
	case EOurImGuiCol::FrameBgActive:         return ImGuiCol_FrameBgActive;
	case EOurImGuiCol::TitleBg:               return ImGuiCol_TitleBg;
	case EOurImGuiCol::TitleBgActive:         return ImGuiCol_TitleBgActive;
	case EOurImGuiCol::TitleBgCollapsed:      return ImGuiCol_TitleBgCollapsed;
	case EOurImGuiCol::MenuBarBg:             return ImGuiCol_MenuBarBg;
	case EOurImGuiCol::ScrollbarBg:           return ImGuiCol_ScrollbarBg;
	case EOurImGuiCol::ScrollbarGrab:          return ImGuiCol_ScrollbarGrab;
	case EOurImGuiCol::ScrollbarGrabHovered:   return ImGuiCol_ScrollbarGrabHovered;
	case EOurImGuiCol::ScrollbarGrabActive:    return ImGuiCol_ScrollbarGrabActive;
	case EOurImGuiCol::CheckMark:             return ImGuiCol_CheckMark;
	case EOurImGuiCol::SliderGrab:            return ImGuiCol_SliderGrab;
	case EOurImGuiCol::SliderGrabActive:       return ImGuiCol_SliderGrabActive;
	case EOurImGuiCol::Button:                return ImGuiCol_Button;
	case EOurImGuiCol::ButtonHovered:         return ImGuiCol_ButtonHovered;
	case EOurImGuiCol::ButtonActive:          return ImGuiCol_ButtonActive;
	case EOurImGuiCol::Header:                return ImGuiCol_Header;
	case EOurImGuiCol::HeaderHovered:         return ImGuiCol_HeaderHovered;
	case EOurImGuiCol::HeaderActive:          return ImGuiCol_HeaderActive;
	case EOurImGuiCol::Separator:             return ImGuiCol_Separator;
	case EOurImGuiCol::SeparatorHovered:      return ImGuiCol_SeparatorHovered;
	case EOurImGuiCol::SeparatorActive:       return ImGuiCol_SeparatorActive;
	case EOurImGuiCol::ResizeGrip:            return ImGuiCol_ResizeGrip;
	case EOurImGuiCol::ResizeGripHovered:     return ImGuiCol_ResizeGripHovered;
	case EOurImGuiCol::ResizeGripActive:      return ImGuiCol_ResizeGripActive;
	case EOurImGuiCol::Tab:                   return ImGuiCol_Tab;
	case EOurImGuiCol::TabHovered:            return ImGuiCol_TabHovered;
	case EOurImGuiCol::TabActive:             return ImGuiCol_TabActive;
	case EOurImGuiCol::TabUnfocused:          return ImGuiCol_TabUnfocused;
	case EOurImGuiCol::TabUnfocusedActive:    return ImGuiCol_TabUnfocusedActive;
	case EOurImGuiCol::PlotLines:             return ImGuiCol_PlotLines;
	case EOurImGuiCol::PlotLinesHovered:      return ImGuiCol_PlotLinesHovered;
	case EOurImGuiCol::PlotHistogram:         return ImGuiCol_PlotHistogram;
	case EOurImGuiCol::PlotHistogramHovered:  return ImGuiCol_PlotHistogramHovered;
	case EOurImGuiCol::TableHeaderBg:         return ImGuiCol_TableHeaderBg;
	case EOurImGuiCol::TableBorderStrong:     return ImGuiCol_TableBorderStrong;
	case EOurImGuiCol::TableBorderLight:      return ImGuiCol_TableBorderLight;
	case EOurImGuiCol::TableRowBg:            return ImGuiCol_TableRowBg;
	case EOurImGuiCol::TableRowBgAlt:         return ImGuiCol_TableRowBgAlt;
	case EOurImGuiCol::TextSelectedBg:        return ImGuiCol_TextSelectedBg;
	case EOurImGuiCol::DragDropTarget:        return ImGuiCol_DragDropTarget;
	case EOurImGuiCol::NavHighlight:          return ImGuiCol_NavHighlight;
	case EOurImGuiCol::NavWindowingHighlight: return ImGuiCol_NavWindowingHighlight;
	case EOurImGuiCol::NavWindowingDimBg:     return ImGuiCol_NavWindowingDimBg;
	case EOurImGuiCol::ModalWindowDimBg:      return ImGuiCol_ModalWindowDimBg;
	default:                                  return ImGuiCol_Text;
	}
}

int32 UImGuiBlueprintBridge::ResolveStyleVarIndex(EOurImGuiStyleVar Var)
{
	switch (Var)
	{
	case EOurImGuiStyleVar::Alpha:               return ImGuiStyleVar_Alpha;
	case EOurImGuiStyleVar::DisabledAlpha:       return ImGuiStyleVar_DisabledAlpha;
	case EOurImGuiStyleVar::WindowPadding:       return ImGuiStyleVar_WindowPadding;
	case EOurImGuiStyleVar::WindowRounding:      return ImGuiStyleVar_WindowRounding;
	case EOurImGuiStyleVar::WindowBorderSize:    return ImGuiStyleVar_WindowBorderSize;
	case EOurImGuiStyleVar::WindowMinSize:       return ImGuiStyleVar_WindowMinSize;
	case EOurImGuiStyleVar::WindowTitleAlign:    return ImGuiStyleVar_WindowTitleAlign;
	case EOurImGuiStyleVar::ChildRounding:       return ImGuiStyleVar_ChildRounding;
	case EOurImGuiStyleVar::ChildBorderSize:     return ImGuiStyleVar_ChildBorderSize;
	case EOurImGuiStyleVar::PopupRounding:       return ImGuiStyleVar_PopupRounding;
	case EOurImGuiStyleVar::PopupBorderSize:     return ImGuiStyleVar_PopupBorderSize;
	case EOurImGuiStyleVar::FramePadding:        return ImGuiStyleVar_FramePadding;
	case EOurImGuiStyleVar::FrameRounding:       return ImGuiStyleVar_FrameRounding;
	case EOurImGuiStyleVar::FrameBorderSize:     return ImGuiStyleVar_FrameBorderSize;
	case EOurImGuiStyleVar::ItemSpacing:         return ImGuiStyleVar_ItemSpacing;
	case EOurImGuiStyleVar::ItemInnerSpacing:    return ImGuiStyleVar_ItemInnerSpacing;
	case EOurImGuiStyleVar::IndentSpacing:       return ImGuiStyleVar_IndentSpacing;
	case EOurImGuiStyleVar::CellPadding:         return ImGuiStyleVar_CellPadding;
	case EOurImGuiStyleVar::ScrollbarSize:       return ImGuiStyleVar_ScrollbarSize;
	case EOurImGuiStyleVar::ScrollbarRounding:   return ImGuiStyleVar_ScrollbarRounding;
	case EOurImGuiStyleVar::GrabMinSize:         return ImGuiStyleVar_GrabMinSize;
	case EOurImGuiStyleVar::GrabRounding:        return ImGuiStyleVar_GrabRounding;
	case EOurImGuiStyleVar::TabRounding:         return ImGuiStyleVar_TabRounding;
	case EOurImGuiStyleVar::ButtonTextAlign:     return ImGuiStyleVar_ButtonTextAlign;
	case EOurImGuiStyleVar::SelectableTextAlign: return ImGuiStyleVar_SelectableTextAlign;
	default:                                     return ImGuiStyleVar_Alpha;
	}
}

static ImGuiDir ResolveDir(EOurImGuiDir Dir)
{
	switch (Dir)
	{
	case EOurImGuiDir::Left:  return ImGuiDir_Left;
	case EOurImGuiDir::Right: return ImGuiDir_Right;
	case EOurImGuiDir::Up:    return ImGuiDir_Up;
	case EOurImGuiDir::Down:  return ImGuiDir_Down;
	default:                  return ImGuiDir_None;
	}
}

static int32 TranslateWindowFlags(int32 Flags)
{
	int32 Out = ImGuiWindowFlags_None;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoTitleBar))             Out |= ImGuiWindowFlags_NoTitleBar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoResize))               Out |= ImGuiWindowFlags_NoResize;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoMove))                 Out |= ImGuiWindowFlags_NoMove;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoScrollbar))            Out |= ImGuiWindowFlags_NoScrollbar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoScrollWithMouse))       Out |= ImGuiWindowFlags_NoScrollWithMouse;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoCollapse))             Out |= ImGuiWindowFlags_NoCollapse;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::AlwaysAutoResize))       Out |= ImGuiWindowFlags_AlwaysAutoResize;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoBackground))           Out |= ImGuiWindowFlags_NoBackground;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoSavedSettings))        Out |= ImGuiWindowFlags_NoSavedSettings;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoMouseInputs))          Out |= ImGuiWindowFlags_NoMouseInputs;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::MenuBar))                Out |= ImGuiWindowFlags_MenuBar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::HorizontalScrollbar))    Out |= ImGuiWindowFlags_HorizontalScrollbar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoFocusOnAppearing))     Out |= ImGuiWindowFlags_NoFocusOnAppearing;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoBringToFrontOnFocus))  Out |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::AlwaysVerticalScrollbar)) Out |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::AlwaysHorizontalScrollbar)) Out |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::AlwaysUseWindowPadding))  Out |= ImGuiWindowFlags_AlwaysUseWindowPadding;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoNavInputs))            Out |= ImGuiWindowFlags_NoNavInputs;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::NoNavFocus))             Out |= ImGuiWindowFlags_NoNavFocus;
	if (Flags & (1 << (int32)EOurImGuiWindowFlags::UnsavedDocument))        Out |= ImGuiWindowFlags_UnsavedDocument;
	return Out;
}

static int32 TranslateInputTextFlags(int32 Flags)
{
	int32 Out = ImGuiInputTextFlags_None;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CharsDecimal))        Out |= ImGuiInputTextFlags_CharsDecimal;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CharsHexadecimal))    Out |= ImGuiInputTextFlags_CharsHexadecimal;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CharsUppercase))      Out |= ImGuiInputTextFlags_CharsUppercase;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CharsNoBlank))        Out |= ImGuiInputTextFlags_CharsNoBlank;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::AutoSelectAll))       Out |= ImGuiInputTextFlags_AutoSelectAll;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::EnterReturnsTrue))    Out |= ImGuiInputTextFlags_EnterReturnsTrue;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CallbackCompletion))  Out |= ImGuiInputTextFlags_CallbackCompletion;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CallbackHistory))     Out |= ImGuiInputTextFlags_CallbackHistory;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CallbackAlways))      Out |= ImGuiInputTextFlags_CallbackAlways;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CallbackCharFilter))  Out |= ImGuiInputTextFlags_CallbackCharFilter;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::AllowTabInput))       Out |= ImGuiInputTextFlags_AllowTabInput;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CtrlEnterForNewLine)) Out |= ImGuiInputTextFlags_CtrlEnterForNewLine;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::NoHorizontalScroll))  Out |= ImGuiInputTextFlags_NoHorizontalScroll;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::AlwaysOverwrite))     Out |= ImGuiInputTextFlags_AlwaysOverwrite;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::ReadOnly))            Out |= ImGuiInputTextFlags_ReadOnly;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::Password))            Out |= ImGuiInputTextFlags_Password;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::NoUndoRedo))          Out |= ImGuiInputTextFlags_NoUndoRedo;
	if (Flags & (1 << (int32)EOurImGuiInputTextFlags::CharsScientific))     Out |= ImGuiInputTextFlags_CharsScientific;
	return Out;
}

static int32 TranslateTreeNodeFlags(int32 Flags)
{
	int32 Out = ImGuiTreeNodeFlags_None;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::Selected))             Out |= ImGuiTreeNodeFlags_Selected;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::Framed))               Out |= ImGuiTreeNodeFlags_Framed;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::AllowItemOverlap))     Out |= ImGuiTreeNodeFlags_AllowItemOverlap;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::NoTreePushOnOpen))     Out |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::NoAutoOpenOnLog))      Out |= ImGuiTreeNodeFlags_NoAutoOpenOnLog;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::DefaultOpen))          Out |= ImGuiTreeNodeFlags_DefaultOpen;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::OpenOnDoubleClick))    Out |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::OpenOnArrow))          Out |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::Leaf))                 Out |= ImGuiTreeNodeFlags_Leaf;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::Bullet))               Out |= ImGuiTreeNodeFlags_Bullet;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::FramePadding))         Out |= ImGuiTreeNodeFlags_FramePadding;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::SpanAvailWidth))       Out |= ImGuiTreeNodeFlags_SpanAvailWidth;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::SpanFullWidth))        Out |= ImGuiTreeNodeFlags_SpanFullWidth;
	if (Flags & (1 << (int32)EOurImGuiTreeNodeFlags::NavLeftJumpsBackHere)) Out |= ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
	return Out;
}

static int32 TranslateTableFlags(int32 Flags)
{
	int32 Out = ImGuiTableFlags_None;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::Resizable))                  Out |= ImGuiTableFlags_Resizable;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::Reorderable))                Out |= ImGuiTableFlags_Reorderable;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::Hideable))                   Out |= ImGuiTableFlags_Hideable;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::Sortable))                   Out |= ImGuiTableFlags_Sortable;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoSavedSettings))            Out |= ImGuiTableFlags_NoSavedSettings;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::ContextMenuInBody))          Out |= ImGuiTableFlags_ContextMenuInBody;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::RowBg))                      Out |= ImGuiTableFlags_RowBg;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::BordersInnerH))              Out |= ImGuiTableFlags_BordersInnerH;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::BordersOuterH))              Out |= ImGuiTableFlags_BordersOuterH;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::BordersInnerV))              Out |= ImGuiTableFlags_BordersInnerV;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::BordersOuterV))              Out |= ImGuiTableFlags_BordersOuterV;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoBordersInBody))            Out |= ImGuiTableFlags_NoBordersInBody;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoBordersInBodyUntilResize)) Out |= ImGuiTableFlags_NoBordersInBodyUntilResize;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SizingFixedFit))             Out |= ImGuiTableFlags_SizingFixedFit;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SizingFixedSame))            Out |= ImGuiTableFlags_SizingFixedSame;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SizingStretchProp))          Out |= ImGuiTableFlags_SizingStretchProp;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SizingStretchSame))          Out |= ImGuiTableFlags_SizingStretchSame;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoHostExtendX))              Out |= ImGuiTableFlags_NoHostExtendX;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoHostExtendY))              Out |= ImGuiTableFlags_NoHostExtendY;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoKeepColumnsVisible))       Out |= ImGuiTableFlags_NoKeepColumnsVisible;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::PreciseWidths))              Out |= ImGuiTableFlags_PreciseWidths;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoClip))                     Out |= ImGuiTableFlags_NoClip;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::PadOuterX))                  Out |= ImGuiTableFlags_PadOuterX;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoPadOuterX))                Out |= ImGuiTableFlags_NoPadOuterX;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::NoPadInnerX))                Out |= ImGuiTableFlags_NoPadInnerX;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::ScrollX))                    Out |= ImGuiTableFlags_ScrollX;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::ScrollY))                    Out |= ImGuiTableFlags_ScrollY;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SortMulti))                  Out |= ImGuiTableFlags_SortMulti;
	if (Flags & (1 << (int32)EOurImGuiTableFlags::SortTristate))               Out |= ImGuiTableFlags_SortTristate;
	return Out;
}

static int32 TranslateTableColumnFlags(int32 Flags)
{
	int32 Out = ImGuiTableColumnFlags_None;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::Disabled))             Out |= ImGuiTableColumnFlags_Disabled;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::DefaultHide))          Out |= ImGuiTableColumnFlags_DefaultHide;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::DefaultSort))          Out |= ImGuiTableColumnFlags_DefaultSort;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::WidthStretch))         Out |= ImGuiTableColumnFlags_WidthStretch;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::WidthFixed))           Out |= ImGuiTableColumnFlags_WidthFixed;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoResize))             Out |= ImGuiTableColumnFlags_NoResize;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoReorder))            Out |= ImGuiTableColumnFlags_NoReorder;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoHide))               Out |= ImGuiTableColumnFlags_NoHide;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoClip))               Out |= ImGuiTableColumnFlags_NoClip;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoSort))               Out |= ImGuiTableColumnFlags_NoSort;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoSortAscending))      Out |= ImGuiTableColumnFlags_NoSortAscending;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoSortDescending))     Out |= ImGuiTableColumnFlags_NoSortDescending;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoHeaderLabel))        Out |= ImGuiTableColumnFlags_NoHeaderLabel;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::NoHeaderWidth))        Out |= ImGuiTableColumnFlags_NoHeaderWidth;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::PreferSortAscending))  Out |= ImGuiTableColumnFlags_PreferSortAscending;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::PreferSortDescending)) Out |= ImGuiTableColumnFlags_PreferSortDescending;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::IndentEnable))         Out |= ImGuiTableColumnFlags_IndentEnable;
	if (Flags & (1 << (int32)EOurImGuiTableColumnFlags::IndentDisable))        Out |= ImGuiTableColumnFlags_IndentDisable;
	return Out;
}

static int32 TranslateTabBarFlags(int32 Flags)
{
	int32 Out = ImGuiTabBarFlags_None;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::Reorderable))                  Out |= ImGuiTabBarFlags_Reorderable;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::AutoSelectNewTabs))            Out |= ImGuiTabBarFlags_AutoSelectNewTabs;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::TabListPopupButton))           Out |= ImGuiTabBarFlags_TabListPopupButton;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::NoCloseWithMiddleMouseButton)) Out |= ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::NoTabListScrollingButtons))    Out |= ImGuiTabBarFlags_NoTabListScrollingButtons;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::NoTooltip))                    Out |= ImGuiTabBarFlags_NoTooltip;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::FittingPolicyResizeDown))      Out |= ImGuiTabBarFlags_FittingPolicyResizeDown;
	if (Flags & (1 << (int32)EOurImGuiTabBarFlags::FittingPolicyScroll))          Out |= ImGuiTabBarFlags_FittingPolicyScroll;
	return Out;
}

static int32 TranslateTabItemFlags(int32 Flags)
{
	int32 Out = ImGuiTabItemFlags_None;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::UnsavedDocument))              Out |= ImGuiTabItemFlags_UnsavedDocument;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::SetSelected))                  Out |= ImGuiTabItemFlags_SetSelected;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::NoCloseWithMiddleMouseButton)) Out |= ImGuiTabItemFlags_NoCloseWithMiddleMouseButton;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::NoPushId))                     Out |= ImGuiTabItemFlags_NoPushId;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::NoTooltip))                    Out |= ImGuiTabItemFlags_NoTooltip;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::NoReorder))                    Out |= ImGuiTabItemFlags_NoReorder;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::Leading))                      Out |= ImGuiTabItemFlags_Leading;
	if (Flags & (1 << (int32)EOurImGuiTabItemFlags::Trailing))                     Out |= ImGuiTabItemFlags_Trailing;
	return Out;
}

static int32 TranslateColorEditFlags(int32 Flags)
{
	int32 Out = ImGuiColorEditFlags_None;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoAlpha))          Out |= ImGuiColorEditFlags_NoAlpha;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoPicker))         Out |= ImGuiColorEditFlags_NoPicker;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoOptions))        Out |= ImGuiColorEditFlags_NoOptions;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoSmallPreview))   Out |= ImGuiColorEditFlags_NoSmallPreview;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoInputs))         Out |= ImGuiColorEditFlags_NoInputs;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoTooltip))        Out |= ImGuiColorEditFlags_NoTooltip;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoLabel))          Out |= ImGuiColorEditFlags_NoLabel;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoSidePreview))    Out |= ImGuiColorEditFlags_NoSidePreview;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoDragDrop))       Out |= ImGuiColorEditFlags_NoDragDrop;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::NoBorder))         Out |= ImGuiColorEditFlags_NoBorder;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::AlphaBar))         Out |= ImGuiColorEditFlags_AlphaBar;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::AlphaPreview))     Out |= ImGuiColorEditFlags_AlphaPreview;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::AlphaPreviewHalf)) Out |= ImGuiColorEditFlags_AlphaPreviewHalf;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::HDR))              Out |= ImGuiColorEditFlags_HDR;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::DisplayRGB))       Out |= ImGuiColorEditFlags_DisplayRGB;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::DisplayHSV))       Out |= ImGuiColorEditFlags_DisplayHSV;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::DisplayHex))       Out |= ImGuiColorEditFlags_DisplayHex;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::Uint8))            Out |= ImGuiColorEditFlags_Uint8;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::Float))            Out |= ImGuiColorEditFlags_Float;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::PickerHueBar))     Out |= ImGuiColorEditFlags_PickerHueBar;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::PickerHueWheel))   Out |= ImGuiColorEditFlags_PickerHueWheel;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::InputRGB))         Out |= ImGuiColorEditFlags_InputRGB;
	if (Flags & (1 << (int32)EOurImGuiColorEditFlags::InputHSV))         Out |= ImGuiColorEditFlags_InputHSV;
	return Out;
}

static int32 TranslateSliderFlags(int32 Flags)
{
	int32 Out = ImGuiSliderFlags_None;
	if (Flags & (1 << (int32)EOurImGuiSliderFlags::AlwaysClamp))     Out |= ImGuiSliderFlags_AlwaysClamp;
	if (Flags & (1 << (int32)EOurImGuiSliderFlags::Logarithmic))     Out |= ImGuiSliderFlags_Logarithmic;
	if (Flags & (1 << (int32)EOurImGuiSliderFlags::NoRoundToFormat)) Out |= ImGuiSliderFlags_NoRoundToFormat;
	if (Flags & (1 << (int32)EOurImGuiSliderFlags::NoInput))         Out |= ImGuiSliderFlags_NoInput;
	return Out;
}

static int32 TranslatePopupFlags(int32 Flags)
{
	int32 Out = ImGuiPopupFlags_None;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::MouseButtonLeft))         Out |= ImGuiPopupFlags_MouseButtonLeft;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::MouseButtonRight))        Out |= ImGuiPopupFlags_MouseButtonRight;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::MouseButtonMiddle))       Out |= ImGuiPopupFlags_MouseButtonMiddle;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::NoOpenOverExistingPopup)) Out |= ImGuiPopupFlags_NoOpenOverExistingPopup;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::NoOpenOverItems))         Out |= ImGuiPopupFlags_NoOpenOverItems;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::AnyPopupId))              Out |= ImGuiPopupFlags_AnyPopupId;
	if (Flags & (1 << (int32)EOurImGuiPopupFlags::AnyPopupLevel))           Out |= ImGuiPopupFlags_AnyPopupLevel;
	return Out;
}

static int32 TranslateSelectableFlags(int32 Flags)
{
	int32 Out = ImGuiSelectableFlags_None;
	if (Flags & (1 << (int32)EOurImGuiSelectableFlags::DontClosePopups))  Out |= ImGuiSelectableFlags_DontClosePopups;
	if (Flags & (1 << (int32)EOurImGuiSelectableFlags::SpanAllColumns))   Out |= ImGuiSelectableFlags_SpanAllColumns;
	if (Flags & (1 << (int32)EOurImGuiSelectableFlags::AllowDoubleClick)) Out |= ImGuiSelectableFlags_AllowDoubleClick;
	if (Flags & (1 << (int32)EOurImGuiSelectableFlags::Disabled))         Out |= ImGuiSelectableFlags_Disabled;
	if (Flags & (1 << (int32)EOurImGuiSelectableFlags::AllowItemOverlap)) Out |= ImGuiSelectableFlags_AllowItemOverlap;
	return Out;
}

static int32 TranslateComboFlags(int32 Flags)
{
	int32 Out = ImGuiComboFlags_None;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::PopupAlignLeft)) Out |= ImGuiComboFlags_PopupAlignLeft;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::HeightSmall))    Out |= ImGuiComboFlags_HeightSmall;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::HeightRegular))  Out |= ImGuiComboFlags_HeightRegular;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::HeightLarge))    Out |= ImGuiComboFlags_HeightLarge;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::HeightLargest))  Out |= ImGuiComboFlags_HeightLargest;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::NoArrowButton))  Out |= ImGuiComboFlags_NoArrowButton;
	if (Flags & (1 << (int32)EOurImGuiComboFlags::NoPreview))      Out |= ImGuiComboFlags_NoPreview;
	return Out;
}

static int32 TranslateHoveredFlags(int32 Flags)
{
	int32 Out = ImGuiHoveredFlags_None;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::ChildWindows))                 Out |= ImGuiHoveredFlags_ChildWindows;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::RootWindow))                  Out |= ImGuiHoveredFlags_RootWindow;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::AnyWindow))                   Out |= ImGuiHoveredFlags_AnyWindow;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::NoPopupHierarchy))            Out |= ImGuiHoveredFlags_NoPopupHierarchy;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::AllowWhenBlockedByPopup))      Out |= ImGuiHoveredFlags_AllowWhenBlockedByPopup;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::AllowWhenBlockedByActiveItem)) Out |= ImGuiHoveredFlags_AllowWhenBlockedByActiveItem;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::AllowWhenOverlapped))          Out |= ImGuiHoveredFlags_AllowWhenOverlapped;
	if (Flags & (1 << (int32)EOurImGuiHoveredFlags::AllowWhenDisabled))            Out |= ImGuiHoveredFlags_AllowWhenDisabled;
	return Out;
}

static int32 TranslateFocusedFlags(int32 Flags)
{
	int32 Out = ImGuiFocusedFlags_None;
	if (Flags & (1 << (int32)EOurImGuiFocusedFlags::ChildWindows))     Out |= ImGuiFocusedFlags_ChildWindows;
	if (Flags & (1 << (int32)EOurImGuiFocusedFlags::RootWindow))       Out |= ImGuiFocusedFlags_RootWindow;
	if (Flags & (1 << (int32)EOurImGuiFocusedFlags::AnyWindow))        Out |= ImGuiFocusedFlags_AnyWindow;
	if (Flags & (1 << (int32)EOurImGuiFocusedFlags::NoPopupHierarchy)) Out |= ImGuiFocusedFlags_NoPopupHierarchy;
	return Out;
}


// ---------------------------------------------------------------------------
// Windows & Layout Containers
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::BeginWindow(const FString& WindowName, int32 WindowFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::Begin(TCHAR_TO_UTF8(*WindowName), nullptr, TranslateWindowFlags(WindowFlags));
}

bool UImGuiBlueprintBridge::BeginWindowWithClose(const FString& WindowName, bool& bOpen, int32 WindowFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	bool bPrevOpen = bOpen;
	bool bRes = ImGui::Begin(TCHAR_TO_UTF8(*WindowName), &bPrevOpen, TranslateWindowFlags(WindowFlags));
	bOpen = bPrevOpen;
	return bRes;
}

void UImGuiBlueprintBridge::EndWindow()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::End();
}

bool UImGuiBlueprintBridge::BeginChild(const FString& ID, FVector2D Size, bool bBorder, int32 WindowFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginChild(TCHAR_TO_UTF8(*ID), ImVec2(Size.X, Size.Y), bBorder, TranslateWindowFlags(WindowFlags));
}

void UImGuiBlueprintBridge::EndChild()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndChild();
}


// ---------------------------------------------------------------------------
// Layout Utilities
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::Separator()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Separator();
}

void UImGuiBlueprintBridge::SameLine(float OffsetFromStartX, float Spacing)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SameLine(OffsetFromStartX, Spacing);
}

void UImGuiBlueprintBridge::NewLine()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::NewLine();
}

void UImGuiBlueprintBridge::Spacing()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Spacing();
}

void UImGuiBlueprintBridge::Dummy(FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Dummy(ImVec2(Size.X, Size.Y));
}

void UImGuiBlueprintBridge::Indent(float IndentWidth)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Indent(IndentWidth);
}

void UImGuiBlueprintBridge::Unindent(float IndentWidth)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Unindent(IndentWidth);
}

void UImGuiBlueprintBridge::BeginGroup()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::BeginGroup();
}

void UImGuiBlueprintBridge::EndGroup()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndGroup();
}

FVector2D UImGuiBlueprintBridge::GetCursorPos()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 Pos = ImGui::GetCursorPos();
	return FVector2D(Pos.x, Pos.y);
}

void UImGuiBlueprintBridge::SetCursorPos(FVector2D LocalPos)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SetCursorPos(ImVec2(LocalPos.X, LocalPos.Y));
}

FVector2D UImGuiBlueprintBridge::GetCursorScreenPos()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 Pos = ImGui::GetCursorScreenPos();
	return FVector2D(Pos.x, Pos.y);
}

void UImGuiBlueprintBridge::SetCursorScreenPos(FVector2D ScreenPos)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SetCursorScreenPos(ImVec2(ScreenPos.X, ScreenPos.Y));
}

FVector2D UImGuiBlueprintBridge::GetContentRegionAvail()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 R = ImGui::GetContentRegionAvail();
	return FVector2D(R.x, R.y);
}

FVector2D UImGuiBlueprintBridge::GetContentRegionMax()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 R = ImGui::GetContentRegionMax();
	return FVector2D(R.x, R.y);
}

void UImGuiBlueprintBridge::AlignTextToFramePadding()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::AlignTextToFramePadding();
}


// ---------------------------------------------------------------------------
// ID Stack
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::PushID_Int(int32 ID)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PushID(ID);
}

void UImGuiBlueprintBridge::PushID_String(const FString& ID)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PushID(TCHAR_TO_UTF8(*ID));
}

void UImGuiBlueprintBridge::PopID()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PopID();
}


// ---------------------------------------------------------------------------
// Text Widgets
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::Text(const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TextUnformatted(TCHAR_TO_UTF8(*TextVal));
}

void UImGuiBlueprintBridge::TextColored(FLinearColor Color, const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TextColored(ImVec4(Color.R, Color.G, Color.B, Color.A), "%s", TCHAR_TO_UTF8(*TextVal));
}

void UImGuiBlueprintBridge::TextDisabled(const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TextDisabled("%s", TCHAR_TO_UTF8(*TextVal));
}

void UImGuiBlueprintBridge::TextWrapped(const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TextWrapped("%s", TCHAR_TO_UTF8(*TextVal));
}

void UImGuiBlueprintBridge::LabelText(const FString& Label, const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::LabelText(TCHAR_TO_UTF8(*Label), "%s", TCHAR_TO_UTF8(*TextVal));
}

void UImGuiBlueprintBridge::BulletText(const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::BulletText("%s", TCHAR_TO_UTF8(*TextVal));
}


// ---------------------------------------------------------------------------
// Buttons & Standard Inputs
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::DebugButton(const FString& Label, FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::Button(TCHAR_TO_UTF8(*Label), ImVec2(Size.X, Size.Y));
}

bool UImGuiBlueprintBridge::SmallButton(const FString& Label)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::SmallButton(TCHAR_TO_UTF8(*Label));
}

bool UImGuiBlueprintBridge::InvisibleButton(const FString& ID, FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::InvisibleButton(TCHAR_TO_UTF8(*ID), ImVec2(Size.X, Size.Y));
}

bool UImGuiBlueprintBridge::ArrowButton(const FString& ID, EOurImGuiDir Dir)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::ArrowButton(TCHAR_TO_UTF8(*ID), ResolveDir(Dir));
}

void UImGuiBlueprintBridge::Checkbox(const FString& Label, bool CurrentValue, bool& OutValue)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Checkbox(TCHAR_TO_UTF8(*Label), &OutValue);
}

bool UImGuiBlueprintBridge::RadioButton(const FString& Label, bool bActive)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::RadioButton(TCHAR_TO_UTF8(*Label), bActive);
}

void UImGuiBlueprintBridge::RadioButtonState(const FString& Label, int32 CurrentValue, int32 ButtonValue, int32& OutValue)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::RadioButton(TCHAR_TO_UTF8(*Label), &OutValue, ButtonValue);
}

void UImGuiBlueprintBridge::ProgressBar(float Fraction, FVector2D Size, const FString& Overlay)
{
	if (!ImGui::GetCurrentContext()) return;
	const char* OverlayStr = Overlay.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Overlay);
	ImGui::ProgressBar(Fraction, ImVec2(Size.X, Size.Y), OverlayStr);
}

void UImGuiBlueprintBridge::Bullet()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Bullet();
}


// ---------------------------------------------------------------------------
// Sliders (Pass-by-Reference Mutation)
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::SliderFloat(const FString& Label, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiSliderSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SliderFloat(TCHAR_TO_UTF8(*Label), &OutValue, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::SliderFloat2(const FString& Label, FVector2D CurrentValue, float Min, float Max, FVector2D& OutValue, const FImGuiSliderSettings& Settings)
{
	float Vals[2] = { (float)CurrentValue.X, (float)CurrentValue.Y };
	if (ImGui::GetCurrentContext())
	{
		ImGui::SliderFloat2(TCHAR_TO_UTF8(*Label), Vals, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector2D(Vals[0], Vals[1]);
}

void UImGuiBlueprintBridge::SliderFloat3(const FString& Label, FVector CurrentValue, float Min, float Max, FVector& OutValue, const FImGuiSliderSettings& Settings)
{
	float Vals[3] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z };
	if (ImGui::GetCurrentContext())
	{
		ImGui::SliderFloat3(TCHAR_TO_UTF8(*Label), Vals, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector(Vals[0], Vals[1], Vals[2]);
}

void UImGuiBlueprintBridge::SliderFloat4(const FString& Label, FVector4 CurrentValue, float Min, float Max, FVector4& OutValue, const FImGuiSliderSettings& Settings)
{
	float Vals[4] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z, (float)CurrentValue.W };
	if (ImGui::GetCurrentContext())
	{
		ImGui::SliderFloat4(TCHAR_TO_UTF8(*Label), Vals, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector4(Vals[0], Vals[1], Vals[2], Vals[3]);
}

void UImGuiBlueprintBridge::SliderInt(const FString& Label, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiSliderSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SliderInt(TCHAR_TO_UTF8(*Label), &OutValue, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::SliderInt2(const FString& Label, int32 CurrentValX, int32 CurrentValY, int32 Min, int32 Max, int32& OutValX, int32& OutValY, const FImGuiSliderSettings& Settings)
{
	int32 Vals[2] = { CurrentValX, CurrentValY };
	if (ImGui::GetCurrentContext())
	{
		ImGui::SliderInt2(TCHAR_TO_UTF8(*Label), Vals, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValX = Vals[0];
	OutValY = Vals[1];
}

void UImGuiBlueprintBridge::VSliderFloat(const FString& Label, FVector2D Size, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiSliderSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::VSliderFloat(TCHAR_TO_UTF8(*Label), ImVec2(Size.X, Size.Y), &OutValue, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::VSliderInt(const FString& Label, FVector2D Size, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiSliderSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::VSliderInt(TCHAR_TO_UTF8(*Label), ImVec2(Size.X, Size.Y), &OutValue, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}


// ---------------------------------------------------------------------------
// Drag Adjusters (Pass-by-Reference Mutation)
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::DragFloat(const FString& Label, float CurrentValue, float Min, float Max, float& OutValue, const FImGuiDragSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::DragFloat(TCHAR_TO_UTF8(*Label), &OutValue, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::DragFloat2(const FString& Label, FVector2D CurrentValue, float Min, float Max, FVector2D& OutValue, const FImGuiDragSettings& Settings)
{
	float Vals[2] = { (float)CurrentValue.X, (float)CurrentValue.Y };
	if (ImGui::GetCurrentContext())
	{
		ImGui::DragFloat2(TCHAR_TO_UTF8(*Label), Vals, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector2D(Vals[0], Vals[1]);
}

void UImGuiBlueprintBridge::DragFloat3(const FString& Label, FVector CurrentValue, float Min, float Max, FVector& OutValue, const FImGuiDragSettings& Settings)
{
	float Vals[3] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z };
	if (ImGui::GetCurrentContext())
	{
		ImGui::DragFloat3(TCHAR_TO_UTF8(*Label), Vals, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector(Vals[0], Vals[1], Vals[2]);
}

void UImGuiBlueprintBridge::DragFloat4(const FString& Label, FVector4 CurrentValue, float Min, float Max, FVector4& OutValue, const FImGuiDragSettings& Settings)
{
	float Vals[4] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z, (float)CurrentValue.W };
	if (ImGui::GetCurrentContext())
	{
		ImGui::DragFloat4(TCHAR_TO_UTF8(*Label), Vals, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValue = FVector4(Vals[0], Vals[1], Vals[2], Vals[3]);
}

void UImGuiBlueprintBridge::DragInt(const FString& Label, int32 CurrentValue, int32 Min, int32 Max, int32& OutValue, const FImGuiDragSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::DragInt(TCHAR_TO_UTF8(*Label), &OutValue, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::DragInt2(const FString& Label, int32 CurrentValX, int32 CurrentValY, int32 Min, int32 Max, int32& OutValX, int32& OutValY, const FImGuiDragSettings& Settings)
{
	int32 Vals[2] = { CurrentValX, CurrentValY };
	if (ImGui::GetCurrentContext())
	{
		ImGui::DragInt2(TCHAR_TO_UTF8(*Label), Vals, Settings.Speed, Min, Max, TCHAR_TO_UTF8(*Settings.Format), TranslateSliderFlags(Settings.Flags));
	}
	OutValX = Vals[0];
	OutValY = Vals[1];
}


// ---------------------------------------------------------------------------
// TextInput Keyboard Fields
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::InputText(const FString& Label, const FString& CurrentValue, FString& OutValue, const FImGuiInputTextSettings& Settings)
{
	// Convert FString to UTF-8 buffer for editing
	ANSICHAR Buffer[2048];
	FCStringAnsi::Strncpy(Buffer, TCHAR_TO_UTF8(*CurrentValue), 2048);

	if (ImGui::GetCurrentContext())
	{
		ImGui::InputText(TCHAR_TO_UTF8(*Label), Buffer, 2048, TranslateInputTextFlags(Settings.Flags));
	}
	OutValue = FString(UTF8_TO_TCHAR(Buffer));
}

void UImGuiBlueprintBridge::InputTextMultiline(const FString& Label, const FString& CurrentValue, FString& OutValue, FVector2D Size, const FImGuiInputTextSettings& Settings)
{
	ANSICHAR Buffer[4096];
	FCStringAnsi::Strncpy(Buffer, TCHAR_TO_UTF8(*CurrentValue), 4096);

	if (ImGui::GetCurrentContext())
	{
		ImGui::InputTextMultiline(TCHAR_TO_UTF8(*Label), Buffer, 4096, ImVec2(Size.X, Size.Y), TranslateInputTextFlags(Settings.Flags));
	}
	OutValue = FString(UTF8_TO_TCHAR(Buffer));
}

void UImGuiBlueprintBridge::InputFloat(const FString& Label, float CurrentValue, float Step, float StepFast, float& OutValue, const FImGuiInputTextSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::InputFloat(TCHAR_TO_UTF8(*Label), &OutValue, Step, StepFast, "%.3f", TranslateInputTextFlags(Settings.Flags));
}

void UImGuiBlueprintBridge::InputFloat2(const FString& Label, FVector2D CurrentValue, FVector2D& OutValue, const FImGuiInputTextSettings& Settings)
{
	float Vals[2] = { (float)CurrentValue.X, (float)CurrentValue.Y };
	if (ImGui::GetCurrentContext())
	{
		ImGui::InputFloat2(TCHAR_TO_UTF8(*Label), Vals, "%.3f", TranslateInputTextFlags(Settings.Flags));
	}
	OutValue = FVector2D(Vals[0], Vals[1]);
}

void UImGuiBlueprintBridge::InputFloat3(const FString& Label, FVector CurrentValue, FVector& OutValue, const FImGuiInputTextSettings& Settings)
{
	float Vals[3] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z };
	if (ImGui::GetCurrentContext())
	{
		ImGui::InputFloat3(TCHAR_TO_UTF8(*Label), Vals, "%.3f", TranslateInputTextFlags(Settings.Flags));
	}
	OutValue = FVector(Vals[0], Vals[1], Vals[2]);
}

void UImGuiBlueprintBridge::InputFloat4(const FString& Label, FVector4 CurrentValue, FVector4& OutValue, const FImGuiInputTextSettings& Settings)
{
	float Vals[4] = { (float)CurrentValue.X, (float)CurrentValue.Y, (float)CurrentValue.Z, (float)CurrentValue.W };
	if (ImGui::GetCurrentContext())
	{
		ImGui::InputFloat4(TCHAR_TO_UTF8(*Label), Vals, "%.3f", TranslateInputTextFlags(Settings.Flags));
	}
	OutValue = FVector4(Vals[0], Vals[1], Vals[2], Vals[3]);
}

void UImGuiBlueprintBridge::InputInt(const FString& Label, int32 CurrentValue, int32 Step, int32 StepFast, int32& OutValue, const FImGuiInputTextSettings& Settings)
{
	OutValue = CurrentValue;
	if (!ImGui::GetCurrentContext()) return;
	ImGui::InputInt(TCHAR_TO_UTF8(*Label), &OutValue, Step, StepFast, TranslateInputTextFlags(Settings.Flags));
}


// ---------------------------------------------------------------------------
// Color Selectors
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::ColorEdit3(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings)
{
	float Vals[3] = { CurrentColor.R, CurrentColor.G, CurrentColor.B };
	if (ImGui::GetCurrentContext())
	{
		ImGui::ColorEdit3(TCHAR_TO_UTF8(*Label), Vals, TranslateColorEditFlags(Settings.Flags));
	}
	OutColor = FLinearColor(Vals[0], Vals[1], Vals[2], CurrentColor.A);
}

void UImGuiBlueprintBridge::ColorEdit4(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings)
{
	float Vals[4] = { CurrentColor.R, CurrentColor.G, CurrentColor.B, CurrentColor.A };
	if (ImGui::GetCurrentContext())
	{
		ImGui::ColorEdit4(TCHAR_TO_UTF8(*Label), Vals, TranslateColorEditFlags(Settings.Flags));
	}
	OutColor = FLinearColor(Vals[0], Vals[1], Vals[2], Vals[3]);
}

void UImGuiBlueprintBridge::ColorPicker3(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings)
{
	float Vals[3] = { CurrentColor.R, CurrentColor.G, CurrentColor.B };
	if (ImGui::GetCurrentContext())
	{
		ImGui::ColorPicker3(TCHAR_TO_UTF8(*Label), Vals, TranslateColorEditFlags(Settings.Flags));
	}
	OutColor = FLinearColor(Vals[0], Vals[1], Vals[2], CurrentColor.A);
}

void UImGuiBlueprintBridge::ColorPicker4(const FString& Label, FLinearColor CurrentColor, FLinearColor& OutColor, const FImGuiColorSettings& Settings)
{
	float Vals[4] = { CurrentColor.R, CurrentColor.G, CurrentColor.B, CurrentColor.A };
	if (ImGui::GetCurrentContext())
	{
		ImGui::ColorPicker4(TCHAR_TO_UTF8(*Label), Vals, TranslateColorEditFlags(Settings.Flags));
	}
	OutColor = FLinearColor(Vals[0], Vals[1], Vals[2], Vals[3]);
}

bool UImGuiBlueprintBridge::ColorButton(const FString& ID, FLinearColor Color, int32 Flags, FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::ColorButton(TCHAR_TO_UTF8(*ID), ImVec4(Color.R, Color.G, Color.B, Color.A), TranslateColorEditFlags(Flags), ImVec2(Size.X, Size.Y));
}


// ---------------------------------------------------------------------------
// Combos & ListBoxes
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::BeginCombo(const FString& Label, const FString& PreviewValue, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginCombo(TCHAR_TO_UTF8(*Label), TCHAR_TO_UTF8(*PreviewValue), TranslateComboFlags(Flags));
}

void UImGuiBlueprintBridge::EndCombo()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndCombo();
}

void UImGuiBlueprintBridge::Combo(const FString& Label, int32 CurrentIndex, const TArray<FString>& Items, int32& OutIndex, int32 HeightInItems)
{
	OutIndex = CurrentIndex;
	if (!ImGui::GetCurrentContext()) return;

	// Populate item pointers
	TArray<std::string> UTF8Strings;
	TArray<const char*> ItemPointers;
	UTF8Strings.Reserve(Items.Num());
	ItemPointers.Reserve(Items.Num());

	for (const FString& Str : Items)
	{
		UTF8Strings.Add(TCHAR_TO_UTF8(*Str));
		ItemPointers.Add(UTF8Strings.Last().c_str());
	}

	ImGui::Combo(TCHAR_TO_UTF8(*Label), &OutIndex, ItemPointers.GetData(), ItemPointers.Num(), HeightInItems);
}

bool UImGuiBlueprintBridge::BeginListBox(const FString& Label, FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginListBox(TCHAR_TO_UTF8(*Label), ImVec2(Size.X, Size.Y));
}

void UImGuiBlueprintBridge::EndListBox()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndListBox();
}

void UImGuiBlueprintBridge::ListBox(const FString& Label, int32 CurrentIndex, const TArray<FString>& Items, int32& OutIndex, int32 HeightInItems)
{
	OutIndex = CurrentIndex;
	if (!ImGui::GetCurrentContext()) return;

	TArray<std::string> UTF8Strings;
	TArray<const char*> ItemPointers;
	UTF8Strings.Reserve(Items.Num());
	ItemPointers.Reserve(Items.Num());

	for (const FString& Str : Items)
	{
		UTF8Strings.Add(TCHAR_TO_UTF8(*Str));
		ItemPointers.Add(UTF8Strings.Last().c_str());
	}

	ImGui::ListBox(TCHAR_TO_UTF8(*Label), &OutIndex, ItemPointers.GetData(), ItemPointers.Num(), HeightInItems);
}


// ---------------------------------------------------------------------------
// Tree Nodes & Selectables
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::TreeNode(const FString& Label, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::TreeNodeEx(TCHAR_TO_UTF8(*Label), TranslateTreeNodeFlags(Flags));
}

void UImGuiBlueprintBridge::TreePop()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TreePop();
}

bool UImGuiBlueprintBridge::CollapsingHeader(const FString& Label, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::CollapsingHeader(TCHAR_TO_UTF8(*Label), TranslateTreeNodeFlags(Flags));
}

bool UImGuiBlueprintBridge::CollapsingHeaderWithClose(const FString& Label, bool& bVisible, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	bool bPrevVisible = bVisible;
	bool bRes = ImGui::CollapsingHeader(TCHAR_TO_UTF8(*Label), &bPrevVisible, TranslateTreeNodeFlags(Flags));
	bVisible = bPrevVisible;
	return bRes;
}

bool UImGuiBlueprintBridge::Selectable(const FString& Label, bool bSelected, int32 Flags, FVector2D Size)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::Selectable(TCHAR_TO_UTF8(*Label), bSelected, TranslateSelectableFlags(Flags), ImVec2(Size.X, Size.Y));
}


// ---------------------------------------------------------------------------
// Tooltips
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::BeginTooltip()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::BeginTooltip();
}

void UImGuiBlueprintBridge::EndTooltip()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndTooltip();
}

void UImGuiBlueprintBridge::SetTooltip(const FString& TextVal)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SetTooltip("%s", TCHAR_TO_UTF8(*TextVal));
}


// ---------------------------------------------------------------------------
// Popups & Modals
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::BeginPopup(const FString& ID, int32 WindowFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginPopup(TCHAR_TO_UTF8(*ID), TranslateWindowFlags(WindowFlags));
}

bool UImGuiBlueprintBridge::BeginPopupModal(const FString& Name, bool& bOpen, int32 WindowFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	bool bPrevOpen = bOpen;
	bool bRes = ImGui::BeginPopupModal(TCHAR_TO_UTF8(*Name), &bPrevOpen, TranslateWindowFlags(WindowFlags));
	bOpen = bPrevOpen;
	return bRes;
}

void UImGuiBlueprintBridge::EndPopup()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndPopup();
}

void UImGuiBlueprintBridge::OpenPopup(const FString& ID, int32 PopupFlags)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::OpenPopup(TCHAR_TO_UTF8(*ID), TranslatePopupFlags(PopupFlags));
}

void UImGuiBlueprintBridge::CloseCurrentPopup()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::CloseCurrentPopup();
}

bool UImGuiBlueprintBridge::IsPopupOpen(const FString& ID, int32 PopupFlags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsPopupOpen(TCHAR_TO_UTF8(*ID), TranslatePopupFlags(PopupFlags));
}


// ---------------------------------------------------------------------------
// Style Push / Pop Stack
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::PushStyleColor(EOurImGuiCol Element, FLinearColor Color)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PushStyleColor(ResolveColorIndex(Element), ImVec4(Color.R, Color.G, Color.B, Color.A));
}

void UImGuiBlueprintBridge::PopStyleColor(int32 Count)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PopStyleColor(FMath::Max(Count, 1));
}

void UImGuiBlueprintBridge::PushStyleVarFloat(EOurImGuiStyleVar Var, float Value)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PushStyleVar(ResolveStyleVarIndex(Var), Value);
}

void UImGuiBlueprintBridge::PushStyleVarVec2(EOurImGuiStyleVar Var, FVector2D Value)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PushStyleVar(ResolveStyleVarIndex(Var), ImVec2(Value.X, Value.Y));
}

void UImGuiBlueprintBridge::PopStyleVar(int32 Count)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::PopStyleVar(FMath::Max(Count, 1));
}


// ---------------------------------------------------------------------------
// Tables (Modern Grid Systems)
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::BeginTable(const FString& ID, int32 ColumnsCount, int32 Flags, FVector2D OuterSize, float InnerWidth)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginTable(TCHAR_TO_UTF8(*ID), ColumnsCount, TranslateTableFlags(Flags), ImVec2(OuterSize.X, OuterSize.Y), InnerWidth);
}

void UImGuiBlueprintBridge::EndTable()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndTable();
}

void UImGuiBlueprintBridge::TableNextColumn()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TableNextColumn();
}

void UImGuiBlueprintBridge::TableNextRow(int32 RowFlags, float MinRowHeight)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TableNextRow(RowFlags, MinRowHeight);
}

void UImGuiBlueprintBridge::TableSetupColumn(const FString& Label, int32 Flags, float InitWidthOrWeight, int32 UserID)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TableSetupColumn(TCHAR_TO_UTF8(*Label), TranslateTableColumnFlags(Flags), InitWidthOrWeight, UserID);
}

void UImGuiBlueprintBridge::TableHeadersRow()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::TableHeadersRow();
}


// ---------------------------------------------------------------------------
// Tab Bars & Items
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::BeginTabBar(const FString& ID, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::BeginTabBar(TCHAR_TO_UTF8(*ID), TranslateTabBarFlags(Flags));
}

void UImGuiBlueprintBridge::EndTabBar()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndTabBar();
}

bool UImGuiBlueprintBridge::BeginTabItem(const FString& Label, bool& bOpen, int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	bool bPrevOpen = bOpen;
	bool bRes = ImGui::BeginTabItem(TCHAR_TO_UTF8(*Label), &bPrevOpen, TranslateTabItemFlags(Flags));
	bOpen = bPrevOpen;
	return bRes;
}

void UImGuiBlueprintBridge::EndTabItem()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::EndTabItem();
}

void UImGuiBlueprintBridge::SetTabItemClosed(const FString& TabOrBarLabel)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::SetTabItemClosed(TCHAR_TO_UTF8(*TabOrBarLabel));
}


// ---------------------------------------------------------------------------
// Columns (Old Layout API)
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::Columns(int32 ColumnsCount, const FString& ID, bool bBorder)
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::Columns(ColumnsCount, ID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*ID), bBorder);
}

void UImGuiBlueprintBridge::NextColumn()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::NextColumn();
}


// ---------------------------------------------------------------------------
// Item State Queries
// ---------------------------------------------------------------------------

bool UImGuiBlueprintBridge::IsItemHovered(int32 Flags)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemHovered(TranslateHoveredFlags(Flags));
}

bool UImGuiBlueprintBridge::IsItemActive()
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemActive();
}

bool UImGuiBlueprintBridge::IsItemFocused()
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemFocused();
}

bool UImGuiBlueprintBridge::IsItemClicked(int32 MouseButton)
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemClicked(MouseButton);
}

bool UImGuiBlueprintBridge::IsItemEdited()
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemEdited();
}

bool UImGuiBlueprintBridge::IsItemDeactivatedAfterEdit()
{
	if (!ImGui::GetCurrentContext()) return false;
	return ImGui::IsItemDeactivatedAfterEdit();
}

FVector2D UImGuiBlueprintBridge::GetItemRectMin()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 R = ImGui::GetItemRectMin();
	return FVector2D(R.x, R.y);
}

FVector2D UImGuiBlueprintBridge::GetItemRectMax()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 R = ImGui::GetItemRectMax();
	return FVector2D(R.x, R.y);
}

FVector2D UImGuiBlueprintBridge::GetItemRectSize()
{
	if (!ImGui::GetCurrentContext()) return FVector2D::ZeroVector;
	ImVec2 R = ImGui::GetItemRectSize();
	return FVector2D(R.x, R.y);
}


// ---------------------------------------------------------------------------
// Demo & Debug Windows
// ---------------------------------------------------------------------------

void UImGuiBlueprintBridge::ShowDemoWindow(bool& bOpen)
{
	if (!ImGui::GetCurrentContext()) return;
	bool bPrev = bOpen;
	ImGui::ShowDemoWindow(&bPrev);
	bOpen = bPrev;
}

void UImGuiBlueprintBridge::ShowMetricsWindow(bool& bOpen)
{
	if (!ImGui::GetCurrentContext()) return;
	bool bPrev = bOpen;
	ImGui::ShowMetricsWindow(&bPrev);
	bOpen = bPrev;
}

void UImGuiBlueprintBridge::ShowAboutWindow(bool& bOpen)
{
	if (!ImGui::GetCurrentContext()) return;
	bool bPrev = bOpen;
	ImGui::ShowAboutWindow(&bPrev);
	bOpen = bPrev;
}

void UImGuiBlueprintBridge::ShowUserGuide()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui::ShowUserGuide();
}
