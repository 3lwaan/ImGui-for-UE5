# Unreal ImGui (Refactored for Unreal Engine 5.6)

[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Unreal Engine](https://img.shields.io/badge/unreal%20engine-5.6-red.svg)](https://www.unrealengine.com)

**Unreal ImGui** is an Unreal Engine plugin that integrates [Dear ImGui](https://github.com/ocornut/imgui) (by Omar Cornut) and [ImPlot](https://github.com/epezent/implot) (by Evan Pezent) into Unreal Engine. 

This repository has been fully refactored, modernized, and tested to compile and package cleanly under **Unreal Engine 5.6** with strict compiler and API deprecation warning compliance. On top of C++ integration, it features a complete, highly-optimized **Blueprint Bridge library** that brings immediate-mode UI development to Unreal's visual scripting system.

---

## ✨ Features

- ⚡ **Immediate-Mode UI in Blueprints:** Create rich debug panels, inspect variables, and tweak settings directly in Blueprint graphs without managing Slate widgets or UMG objects.
- 📈 **Integrated Plotting (ImPlot):** Built-in support for rendering high-performance 1D/2D graphs, scatter plots, histograms, and heatmaps.
- ⚙️ **Refactored for UE 5.6:** Complete fixes for modern Slate coordinates, `FViewport` encapsulation, explicit vector precision casts, and deprecated warning-level settings in `UnrealBuildTool` (UBT).
- 🖼️ **Flexible Textures:** Register and render standard `UTexture2D` instances, Dynamic Render Targets (`UTextureRenderTarget2D`), or Scene Capture viewports.
- 🔤 **Custom Fonts:** Merge custom glyphs and icon sets (like FontAwesome) directly into the ImGui font atlas.

---

## 🛠️ Installation & C++ Setup

1. Copy the `ImGui` plugin folder into your project's `Plugins/` directory (e.g., `[MyProject]/Plugins/ImGui`).
2. Add `"ImGui"` to the `PublicDependencyModuleNames` or `PrivateDependencyModuleNames` in your project's `[GameName].Build.cs` file.
3. **ImPlot Linking (Required):** To compile correctly against the built-in ImPlot symbols, add this private definition to your `[GameName].Build.cs` file:
   ```csharp
   // Tell the compiler to import ImPlot symbols when linking
   PrivateDefinitions.Add(string.Format("IMPLOT_API=DLLIMPORT"));
   ```
4. Regenerate your project file and build.

---

## 📘 Blueprint Developer Guide

The **ImGui Blueprint Function Library** provides a modular, flexible, and robust way to construct diagnostic menus. 

### 📐 Core Blueprint Architecture

To keep node graphs clean and readable, the bridge follows three primary layout patterns:

#### 1. Dropdown Flag Bitmasks
Instead of cluttering widget nodes with dozens of boolean pins, configuration flags are exposed as native Blueprint bitmasks using enums. 
* You can configure properties (like disabling title bars, enabling search, or altering resize rules) by selecting simple checkboxes from a dropdown menu on the node itself.
* **Supported Flags:** `WindowFlags`, `TableFlags`, `InputTextFlags`, `TreeNodeFlags`, `ColorEditFlags`, `ComboFlags`, `HoveredFlags`, and `FocusedFlags`.

#### 2. Collapsible Settings (`AdvancedDisplay`)
Secondary options (like string formats, logarithmic curves, and step sizes) are encapsulated inside configuration structures:
* `FImGuiSliderSettings`, `FImGuiDragSettings`, `FImGuiInputTextSettings`, `FImGuiColorSettings`.
* These options are marked as **Advanced Display**. By default, the nodes appear tiny and compact. To configure advanced details, click the expand arrow at the bottom of the node.

#### 3. Pass-by-Reference Data Flow
Immediate-mode widgets (like sliders, checkboxes, color pickers, and text inputs) update state dynamically:
* Wire a variable to `CurrentValue`.
* Plug the same variable (or another parameter) into the corresponding `OutValue` reference pin.
* On the same frame, the node will write the modified value back, allowing you to write the updated state directly into your variable.

#### 4. Context Safety Guards (Anti-Crash)
* Blueprint graphs run ticks constantly. If a developer accidentally runs an ImGui node outside the active rendering callback (e.g., in a normal actor `Tick` or `BeginPlay`), **the library will safely ignore the call** rather than crashing the Unreal Editor or Game.

---

### 🎨 Blueprint Usage Examples

#### Creating a Simple Debug Window
To draw a window, wrap your widgets in a `Begin Window` / `End Window` block. The `Begin Window` node returns `True` if the window is open and expanded.
```
  [Event Tick / ImGui Draw Callback]
                 │
                 ▼
       ┌───────────────────┐
       │   Begin Window    ├──────► [Is Expanded? (Branch)]
       │ Name: "My Debug"  │                  │
       └───────────────────┘                  ▼ (True)
                                      ┌──────────────┐
                                      │  Checkbox    │
                                      │ Label: "Show"│
                                      └──────┬───────┘
                                             │
                                             ▼
                                      ┌──────────────┐
                                      │  End Window  │
                                      └──────────────┘
```

#### Slider and Drag Controls
Expose floating-point and integer parameters with automatic step bounds:
- **`SliderFloat` / `SliderInt`:** Restricts values to a slider bar.
- **`DragFloat` / `DragInt`:** Allows developers to click and drag horizontally to increase/decrease values rapidly.

#### Color Pickers & Styling
Expose color parameters with `ColorEdit4` or `ColorPicker4` (using `FLinearColor`). You can also push/pop style modifications to colors and spacing:
- Use `PushStyleColor` before drawing widgets to set properties like button hover states or window backgrounds.
- Always match every `Push` with a `Pop` (e.g., `PopStyleColor(1)`) to avoid pushing style corruptions onto the layout stack.

#### Tables (Grids)
Generate clean diagnostics grids:
1. Call `BeginTable` specifying column counts and boundaries.
2. Call `TableSetupColumn` to name columns and set resize weights.
3. Draw a row using `TableHeadersRow`.
4. Cycle through items, calling `TableNextRow` and `TableNextColumn` before drawing the text or button widgets.
5. End with `EndTable`.

---

## 💻 C++ Developer Guide

### Drawing a Scene Capture / Render Target (`UTextureRenderTarget2D`)
Exposing dynamic game viewports or camera feeds onto ImGui windows is easy using texture interop registration:

```cpp
// 1. Initialize and register the render target
void FMyClass::Init()
{
    TextureRenderTarget = NewObject<UTextureRenderTarget2D>();
    if (IsValid(TextureRenderTarget))
    {
        TextureRenderTarget->InitAutoFormat(512, 512);
        TextureRenderTarget->UpdateResourceImmediate(true);
    }

    // Register with the ImGui module using a unique FName identifier
    ImGuiTextureHandle = FImGuiModule::Get().RegisterTexture(TEXT("MyCaptureTexture"), TextureRenderTarget, true);
}

// 2. Clean up resources at shutdown
FMyClass::~FMyClass()
{
    if (ImGuiTextureHandle.IsValid())
    {
        FImGuiModule::Get().ReleaseTexture(ImGuiTextureHandle);
    }
}

// 3. Render the dynamic quad inside an ImGui Window
void FMyClass::OnImGuiDraw()
{
    if (ImGuiTextureHandle.IsValid())
    {
        ImGui::Begin("Camera Viewport");
        ImGui::Image(ImGuiTextureHandle.GetTextureId(), ImVec2(512.f, 512.f));
        ImGui::End();
    }
}
```

*Note: Set your Scene Capture Component's Capture Source to `Final Color (with tone curve) in Linear sRGB gamut` to ensure correct rendering and transparency.*

### Adding Custom Fonts & Monospaced Icons (FontAwesome)
You can embed custom icon glyph arrays inside your ImGui workspace:

```cpp
#include "FontAwesomeFont.h" // Binary C data of FontAwesome TTF
#include "IconsFontAwesome6.h" // Icon definitions macro header

void FMyClass::SetupCustomFonts()
{
    if (TSharedPtr<ImFontConfig> FAFontConfig = MakeShareable(new ImFontConfig()))
    {
        static const ImWchar IconRange[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        
        FAFontConfig->FontDataOwnedByAtlas = false;
        FAFontConfig->FontData             = (void*)FontAwesomeData;
        FAFontConfig->FontDataSize         = FontAwesomeSize;
        FAFontConfig->SizePixels           = 16;
        FAFontConfig->MergeMode            = true; // Merges into the default typeset atlas
        FAFontConfig->GlyphRanges          = IconRange;
        FAFontConfig->PixelSnapH           = true;
        FAFontConfig->GlyphOffset          = ImVec2(0.f, 3.f);

        FImGuiModule::Get().GetProperties().AddCustomFont("FontAwesome", FAFontConfig);
        FImGuiModule::Get().RebuildFontAtlas();
    }
}
```

---

## 👥 Original Authors & References

- **Base Project Repository:** [segross/UnrealImGui](https://github.com/segross/UnrealImGui)
- **Core Library:** [ocornut/imgui](https://github.com/ocornut/imgui) (Dear ImGui)
- **Plotting Library:** [epezent/implot](https://github.com/epezent/implot) (ImPlot)

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
