# Copilot Instructions for OpenGL City Scene Project

## Project Overview
This project is a C++17 OpenGL application using CMake. It renders a 3D city scene with skybox, multi-light system (moonlight, street lamps, flashlight), and interactive controls.
- **Core Stack**: C++17, OpenGL 3.3+ Core Profile, CMake.
- **Key Libraries**: GLFW (window/input), GLAD (loader), GLM (math), stb_image (textures), ImGui (UI), Assimp (model loading).
- **Entry Point**: `src/main.cpp`.

## Architecture & Patterns

### Directory Structure & Separation of Concerns
- **`src/`**: Contains application logic and scene implementations.
    - **`main.cpp`**: Handles GLFW initialization, input processing (WASD, mouse, P/G/ESC keys), ImGui control panel, and main render loop.
    - **`resources/`**: Resource loading utilities.
        - **`texture.cpp`**: Texture loading wrappers using STB Image (`loadTexture2D`, `createTextureFromData`, `loadSkyboxTexture`).
        - **`model.cpp`**: Assimp-based model loader supporting glTF, OBJ, FBX and 40+ formats.
    - **`scene/`**: Contains scene logic and components.
        - **`city_scene.cpp`**: High-level scene composition with lighting system (moonlight arc, street lamps, flashlight).
        - **`mesh.cpp`**: Mesh class for managing VAO/VBO/EBO with indexed drawing.
        - **`skybox.cpp`**: Equirectangular HDRI skybox rendering with spherical mapping.
- **`include/`**: Header files for all classes.
    - **`camera.hpp`**: FPS camera with mouse/keyboard controls.
    - **`shader.hpp`**: Shader compilation and uniform management.
    - **`model.hpp`**: Model class with Assimp integration.
    - **`mesh.hpp`**: Mesh class with Vertex struct (Position, Normal, TexCoords).
    - **`city_scene.hpp`**: CityScene class with lighting control interfaces.
    - **`skybox.hpp`**: Skybox class declaration.
    - **`texture.hpp`**: Texture loading function declarations.
- **`shader/`**: GLSL source files.
    - **`shader.vert/frag`**: Main shader with multi-light support (DirLight, PointLight, SpotLight).
    - **`skybox.vert/frag`**: Equirectangular skybox shader with spherical mapping.
    - **`lampshader.vert/frag`**: Light source visualization shader.
- **`resource/`**: Assets including textures, HDRI skyboxes, and 3D models (glTF).

### Lighting System
The project implements a comprehensive multi-light system:
- **Directional Light (Moon)**: Cold blueish moonlight with configurable arc trajectory (0-180°).
- **Point Lights (8 Street Lamps)**: Warm orange street lamps with individual on/off controls.
- **Spotlight (Flashlight)**: First-person flashlight attached to camera, toggle with G key.

### Resource Loading Pattern
- **Texture Loading**: Use `include/texture.hpp`.
    - `loadTexture2D`: Loads standard image files with vertical flip.
    - `loadSkyboxTexture`: Loads skybox HDRI without vertical flip.
    - `createTextureFromData`: Creates textures from raw byte data.
    - **Critical**: `STB_IMAGE_IMPLEMENTATION` is defined in `src/resources/texture.cpp`.
- **Model Loading**: Use `include/model.hpp` with Assimp.
    - `Model(path)`: Loads glTF, OBJ, FBX, and other formats automatically.
    - Supports diffuse textures and glTF baseColor textures.
    - Uses `std::filesystem::path` for cross-platform path handling.
- **Path Resolution**: Use `std::filesystem::path` for all file I/O.
- **Macros**: Use CMake-injected preprocessor definitions:
    - `SHADER_DIR`: Absolute path to `shader/` directory.
    - `TEXTURE_DIR`: Absolute path to `resource/` directory.
- **Example**:
    ```cpp
    std::string path = (std::filesystem::path(TEXTURE_DIR) / "CITY" / "scene.gltf").string();
    cityModel = std::make_unique<Model>(path);
    ```

### Rendering Loop
1. **Init**: Initialize GLFW, GLAD, and ImGui. Load CityScene (model, ground plane, skybox).
2. **Update**: Calculate `deltaTime`, process input (WASD movement, mouse look, key toggles).
3. **Render**:
    - Clear buffers (`glClear`).
    - Update flashlight position from camera.
    - Bind main shader, set light uniforms (moon, street lamps, flashlight).
    - Call `cityScene.renderScene(...)` for ground and city model.
    - Call `cityScene.renderSkybox(...)` last with depth test LEQUAL.
    - Render ImGui control panel if enabled (P key).
4. **Swap**: `glfwSwapBuffers`.

### Control Panel (P key)
- **Performance**: FPS and frame time display.
- **Camera**: Position (X, Y, Z) and orientation (Yaw, Pitch).
- **Moon Light**: Arc angle slider (0-180°), orbit radius, intensity.
- **Street Lamps**: Individual toggles for 8 lamps (L1-L4, R1-R4).
- **Flashlight**: On/off toggle with G key shortcut.

### Modern OpenGL Practices
- **Strict Core Profile**: Do not use immediate mode (`glBegin`/`glEnd`).
- **Geometry Management**: Use the `Mesh` class for indexed drawing with VAO/VBO/EBO (RAII).
- **Shader Wrapper**: Use the `Shader` class for loading shaders and setting uniforms.
- **Math**: Use `glm` for all vector and matrix operations.

## Build & Run
- **Build System**: CMake (out-of-source build recommended).
- **Dependencies**: Fetched automatically via CMake FetchContent (GLFW, GLM, Assimp, ImGui).
- **Commands**:
    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build --target LearningOpenGL -j 6
    ./build/bin/LearningOpenGL
    ```
- **VS Code Tasks**: Use "CMake Configure" and "CMake Build" tasks.

## Controls
- **WASD**: Move camera horizontally.
- **Space/Shift**: Move up/down.
- **Mouse**: Look around (when control panel is hidden).
- **Scroll**: Zoom (adjust FOV).
- **P**: Toggle control panel (shows cursor).
- **G**: Toggle flashlight.
- **ESC**: Pause (click to resume).
- **Arrow Keys**: Fine-tune view angle.

## Coding Style
- **C++ Standard**: C++17. Use `auto`, `constexpr`, and standard smart pointers.
- **Headers**: Use `#pragma once`.
- **Logging**: Use `std::cerr` for errors and `std::cout` for info.
