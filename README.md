<h1 align="center">GAT350-OpenGL</h1>

<p align="center">
  <em>A teaching-focused C++ / OpenGL engine and renderer for GAT350 – Computer Graphics.</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge" alt="C++17">
  <img src="https://img.shields.io/badge/Graphics-OpenGL-Core?style=for-the-badge" alt="OpenGL">
  <img src="https://img.shields.io/badge/GUI-ImGui-5C6BC0?style=for-the-badge" alt="ImGui">
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge" alt="Windows">
</p>

---

## Introduction

This is a comprehensive 3D graphics engine developed in C++ using modern OpenGL, created as part of the GAT350 course (Fall 2025: Q5). The project demonstrates a full-featured rendering pipeline with a **component-driven architecture**, advanced shader support, multiple texture mapping techniques, and a flexible material system.

The engine features a **scene-based architecture** with JSON serialization, a component-driven design pattern (`Scene → Actor → Component`), a resource management system, and support for complex 3D model loading via Assimp. Key systems include:

- Flexible camera system (perspective/orthographic)
- Multiple light types (point / directional / spot)
- Render-to-texture and post-processing
- Shadow-mapping support
- Environment mapping (skybox, reflection, refraction)
- Integrated ImGui editor for real-time scene manipulation

It serves as both a learning platform for computer graphics principles and a demonstration of production-style rendering techniques.

---

## Features

### Core Rendering System

- **Modern OpenGL** renderer with GLAD function loading  
- **Component-based framework** (Actors with modular Components)  
- **Scene Management**
  - JSON-driven scene loading and serialization (`Assets/Scenes/scene0X.json`)
- **Shader System**
  - Vertex & fragment shaders in GLSL
  - Program files (`*.prog`) describing shader combinations and default uniforms
- **Material System**
  - Base color & texture mapping
  - Specular maps for highlights
  - Emissive maps for glowing surfaces
  - Normal maps for detailed surface geometry
  - Cubemap textures for reflections & skyboxes
  - Shadow map support
- **Model Loading**
  - Assimp integration (`squirrel.glb` and others)
- **Texture Management**
  - SDL3_image-based loading of PNG, JPG, BMP, etc.
  - Multiple texture units for base/specular/emissive/normal/cubemap/shadow

### Component System

Located in `Source/Engine/Components/`:

- `CameraComponent`
  - Perspective and orthographic projection
  - Exposes FOV, near/far plane, and clear color
- `LightComponent`
  - Point, directional, and spot lights
  - Color, intensity, range
  - Inner/outer spot angles
  - Shadow-caster toggle
- `ModelRenderer`
  - Draws `Model` instances with bound `Material`
- `FirstPersonController`
  - WASD + mouse look controls
  - Configurable speed & sensitivity
- `RotationComponent`
  - Per-frame rotation for spinning objects
- `PostProcessComponent`
  - Hooks into post-processing pass (screen-space effects)

### Advanced Graphics Features

- **Multiple Light Types**
  - Point lights with distance attenuation
  - Directional lights for sun / moon style lighting
  - Spot lights with inner & outer cones
- **Camera System**
  - Perspective projection for 3D scenes
  - Orthographic projection for special passes
- **Render-to-Texture (RTT)**
  - `RenderTexture` wrapper for OpenGL framebuffers
  - Color+depth and depth-only variants
- **Post-Processing Pipeline**
  - Fullscreen quad rendering via `postprocess.*` shaders
  - Easy to add grayscale, tinting, edge detection, etc.
- **Shadow Mapping**
  - Depth-only shaders (`depth.vert/.frag`) for shadow maps
  - Shadow-aware Phong shader (`lit_phong_shadow.*`)
- **Cubemap & Skybox**
  - `CubeMap` class for 6-sided textures
  - `skybox.mat` & `cubemap.frag/vert` for environment background
- **Normal Mapping**
  - Tangent-space normal maps (e.g., `rocks_normal.jpg`)
  - Tangents generated in model import step
- **Vertex Buffer System**
  - `VertexBuffer` resource with VAO/VBO/IBO management
  - Indexed rendering for efficiency

### Material Parameters

`Renderer/Material.h` defines a compact parameter system:

- Bitfield flags for:
  - `BaseMap`, `SpecularMap`, `EmissiveMap`, `NormalMap`, `CubeMap`, `ShadowMap`
- Scalar & vector parameters:
  - `shininess` (specular exponent)
  - `ior` (index of refraction)
  - `tiling` and `offset` (UV control)
  - `baseColor`, `emissiveColor`

Material assets live in `Build/Assets/Materials/*.mat`  
(e.g., `rocks.mat`, `spot.mat`, `reflection.mat`, `refraction.mat`, `squirrel.mat`).

### Resource Management

- `ResourceManager` (in `Engine/Resources`) for:
  - Automatic loading & caching of textures, materials, shaders, models, audio
- Resources referenced by string keys (e.g., `"renderTexture"`)
- JSON-based configuration for scenes, materials, and shader programs
- ImGui-driven inspectors for runtime adjustment

---

## Installation

### Prerequisites

**Libraries (already included in `Source/ThirdParty/` & `Build/`):**

- **SDL3** – windowing, input, OpenGL context  
- **SDL3_image** – image loading  
- **SDL3_ttf** – font rendering  
- **GLAD** – OpenGL loader  
- **GLM** – math library (vectors, matrices, quaternions)  
- **Assimp** – model loading  
- **FMOD** – audio playback  
- **Dear ImGui** + ImGuiFileDialog – in-engine editor  
- **RapidJSON** – JSON parsing / serialization  

**Development Environment**

- Windows 10 or 11  
- Visual Studio 2019/2022 (x64)  
- GPU + drivers capable of modern OpenGL

### Getting the Project

If working from the `.zip` used in class:

1. Extract it somewhere, e.g.:

   ```text
   C:\Projects\GAT350-OpenGL\
   ```

2. You should see:

   * `OpenGL.sln`
   * `Source/`
   * `Build/`
   * `Build/Assets/` (scenes, shaders, materials, textures, audio)
   * `Build/Application.exe`

If you want to use Git instead:

```bash
git clone https://github.com/TwistedFury/GAT350-OpenGL.git
cd GAT350-OpenGL
```

---

## Building

1. Open **`OpenGL.sln`** in Visual Studio.
2. Select:

   * Configuration: `Debug` or `Release`
   * Platform: `x64`
3. Build with **Build → Build Solution** or `Ctrl + Shift + B`.

The default setup uses the pre-configured third-party source in `Source/ThirdParty`, so you do **not** need to install SDL, GLM, or ImGui separately.

---

## Running

### Option 1 – Prebuilt Binary (Fastest)

1. Go to the `Build/` directory.
2. Ensure these files are present:

   * `Application.exe`
   * `Assets/` folder
   * `SDL3.dll`, `SDL3_image.dll`, `SDL3_ttf.dll`
   * `assimp-vc143-mt.dll`
   * `fmod.dll`
3. Double-click `Application.exe`.

> The executable expects the working directory to be `Build/`, and then switches to `Assets/` at runtime so it can load scenes, shaders, and textures.

### Option 2 – From Visual Studio

1. In Solution Explorer, set the **Application** project as the Startup Project.
2. In Project Properties → Debugging, set **Working Directory** to `$(SolutionDir)Build`.
3. Press **F5** (debug) or **Ctrl+F5** (run without debugger).

---

## Usage

### Controls (Default)

> Controls are implemented in `FirstPersonController` and can be customized there.

* **W / A / S / D** – Move forward / left / back / right
* **Q / E** – Move down / up
* **Right Mouse + Move** – Look around (yaw + pitch)
* **` (Grave Accent)** – Toggle ImGui editor overlay
* **Esc** – Quit application

### Sample Scenes

Scene files are in `Build/Assets/Scenes/`:

* `scene01.json` – basic test scene (good for debugging)
* `scene02.json` – moon / lighting showcase
* `scene03.json` – skybox + reflective/refractive squirrel / cow materials with lights

The default scene loaded in `Main.cpp` is:

```cpp
auto scene = std::make_unique<neu::Scene>();
scene->Load("scenes/scene03.json");
scene->Start();
```

Change the filename above to switch which scene loads by default.

### Editor / Inspector

Once the application is running:

1. Press **`** to open the ImGui editor.
2. Use the **Scene** window to select actors (e.g., `skybox`, `camera`, `spot`, `light01`, `squirrel`).
3. Use the **Inspector** window to:

   * Edit transforms (position, rotation, scale)
   * Adjust light color, intensity, range, and spot angles
   * Change material textures & parameters
   * Configure post-processing settings

---

## Configuration Examples

### Scene JSON (Conceptual)

Actors and components are configured via JSON. The actual files (`scene01.json`, etc.) follow this pattern:

```json
{
  "actors": [
    {
      "name": "camera",
      "components": [
        {
          "type": "CameraComponent",
          "projection": "perspective",
          "fov": 70.0,
          "near": 0.1,
          "far": 100.0
        },
        {
          "type": "FirstPersonController",
          "speed": 5.0,
          "sensitivity": 0.1
        }
      ]
    },
    {
      "name": "light01",
      "components": [
        {
          "type": "LightComponent",
          "lightType": "spot",
          "color": [1.0, 1.0, 1.0],
          "intensity": 1.0,
          "range": 25.0,
          "innerSpotAngle": 10.0,
          "outerSpotAngle": 30.0
        }
      ]
    }
  ]
}
```

### Material JSON

Material files in `Assets/Materials/*.mat` are structured roughly as:

```json
{
  "program": "shaders/lit_phong.prog",
  "baseMap": "textures/spot_diffuse.png",
  "specularMap": "textures/spot_diffuse.png",
  "emissiveMap": "textures/spot_diffuse.png",
  "normalMap": "textures/spot_diffuse.png",
  "cubeMap": "textures/sf_cubemap",
  "baseColor": [1.0, 1.0, 1.0],
  "emissiveColor": [0.0, 0.0, 0.0],
  "shininess": 42.0,
  "tiling": [1.0, 1.0],
  "offset": [0.0, 0.0],
  "ior": 1.7
}
```

### Program JSON

Program files define which shaders to use:

```json
{
  "vertex_shader": "shaders/lit_phong.vert",
  "fragment_shader": "shaders/lit_phong.frag"
}
```

---

## Technical Details

### Component & Scene Framework

In `Source/Engine/Framework/`:

* `Object` – base class with common lifetime helpers
* `Actor` – transform + list of `Component` instances
* `Component` – base class (with `Read`, `Initialize`, `Update`, `UpdateGui`)
* `Scene` – owns and updates all actors, handles JSON loading and saving

**Life cycle:**

1. `Read(json)` – deserialize from scene file
2. `Initialize()` / `Start()` – once at scene start
3. `Update(dt)` – per-frame logic
4. `UpdateGui()` – optional ImGui editor UI

### Rendering Pipeline Overview

1. **Engine Update**

   * Time, input, audio, etc.
2. **Scene Update**

   * Actors & components update (controllers, animators, etc.)
3. **Shadow Pass (if enabled)**

   * Scene rendered from light POV into depth `RenderTexture`
4. **Main Pass**

   * Camera calculates view/projection matrices
   * Each `ModelRenderer` draws its mesh with its `Material` & `Program`
5. **Post-Process Pass**

   * Scene optionally rendered to off-screen `RenderTexture`
   * Fullscreen quad drawn with `postprocess` shader
6. **GUI Pass**

   * ImGui windows rendered on top
7. **Present**

   * Swap buffers via SDL/OpenGL

### Vertex Layout

Models are imported through Assimp and packed into vertex buffers with attributes like:

```cpp
struct Vertex {
    glm::vec3 position;  // layout(location = 0)
    glm::vec2 texcoord;  // layout(location = 1)
    glm::vec3 normal;    // layout(location = 2)
    glm::vec3 tangent;   // layout(location = 3)
};
```

The `VertexBuffer` API sets these attributes and manages VAO/VBO/IBO lifetimes.

### Lighting Implementation

`LightComponent` exposes:

* `LightType` – `Point`, `Directional`, or `Spot`
* `color`, `intensity`, `range`
* `innerSpotAngle`, `outerSpotAngle`
* `shadowCaster` flag

Shaders receive uniform data from `LightComponent::SetProgram`, enabling Phong or Blinn-Phong shading with multiple light types in a single shader path.

---

## Visual Showcase

> Add your own screenshots to the repository (for example under `docs/images/`) and update these paths.

### Environment-Mapped Squirrel / Cow

<p align="center">
  <img src="docs/images/reflection-cow.png" alt="Reflective character with ocean skybox" width="650">
  <br/>
  <em>Real-time reflection using cubemap environment mapping.</em>
</p>


### Diffuse Cow with Material Inspector

<p align="center">
  <img src="docs/images/diffuse-cow.png" alt="Diffuse cow with ImGui material inspector" width="650">
  <br/>
  <em>Inspector editing material textures, shininess, and index of refraction at runtime.</em>
</p>

### Phong-Lit Moon

<p align="center">
  <img src="docs/images/moon.png" alt="Phong-lit moon model" width="650">
  <br/>
  <em>Phong shading on a textured sphere with a single light source.</em>
</p>

---

## Attribution

### Third-Party Libraries

* **SDL3** – windowing, input, and context management
* **SDL3_image** – texture loading
* **SDL3_ttf** – font rendering
* **GLAD** – OpenGL function loading
* **GLM** – math utilities
* **Assimp** – model importing
* **FMOD** – audio playback
* **Dear ImGui** + **ImGuiFileDialog** – in-engine editor UI
* **RapidJSON** – JSON parsing

Each library is included with its own license under `Source/ThirdParty/`.

### Assets

* **Cubemap / Skybox** – `Assets/Textures/sf_cubemap/*`

  * Author: Emil Persson (Humus) – [http://www.humus.name](http://www.humus.name)
  * License: Creative Commons Attribution 3.0 Unported

Other models, textures, and fonts are for educational use within this course; check their original sources if re-using them outside the project.

---

## Project Structure (High-Level)

```text
GAT350-OpenGL/
├── Build/
│   ├── Application.exe           # Compiled app
│   ├── Assets/
│   │   ├── Scenes/               # JSON scenes (scene01/02/03)
│   │   ├── Shaders/              # GLSL shaders + .prog files
│   │   ├── Materials/            # .mat material definitions
│   │   ├── Models/               # 3D models (e.g., squirrel.glb)
│   │   ├── Textures/             # 2D textures + cubemaps
│   │   └── test.wav              # Audio clip for FMOD
│   └── *.dll                     # SDL, Assimp, FMOD, etc.
├── Source/
│   ├── Application/              # Main entry (`Main.cpp`)
│   └── Engine/
│       ├── Audio/                # Audio system
│       ├── Components/           # Camera, light, model renderer, etc.
│       ├── Core/                 # Time, logging, JSON helpers, file
│       ├── Event/                # Event system
│       ├── Framework/            # Scene, Actor, Component
│       ├── GUI/                  # Editor integration
│       ├── Input/                # Input handling
│       ├── Math/                 # Math helpers
│       ├── Physics/              # (stubs / future use)
│       ├── Renderer/             # Material, Model, Program, Texture, VertexBuffer
│       └── Resources/            # Resource & ResourceManager
└── OpenGL.sln                     # Visual Studio solution
```

---

## Conclusion

This project represents a solid exploration of modern OpenGL graphics programming and small-engine architecture:

* Component-based scene system
* Shader-driven materials & lighting
* Render-to-texture and post-processing
* Environment mapping and normal mapping
* Integrated tooling with ImGui

### Possible Extensions

* Physically Based Rendering (PBR) workflow
* HDR + tone mapping and bloom
* Deferred rendering and SSAO
* Skeletal animation & particles
* Instanced rendering and culling for large scenes

Use this as a foundation for your own experiments in graphics, tools, and engine design.

---

## Copyright

© **2025 Cody Owens**
All rights reserved.

README authored with assistance from **ChatGPT (GPT-5.1 Thinking)** by OpenAI.

Third-party libraries and assets are the property of their respective owners and are used under their own licenses.
