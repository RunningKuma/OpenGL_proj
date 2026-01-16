# Copilot Instructions for OpenGL City Scene Project

## Project Overview
This project is a C++17 OpenGL application using CMake. It renders a 3D city scene with skyboxes, textures, and lighting.
- **Core Stack**: C++17, OpenGL 3.3+ Core Profile, CMake.
- **Key Libraries**: GLFW (window/input), GLAD (loader), GLM (math), stb_image (textures), ImGui (UI).
- **Entry Point**: `src/main.cpp`.

## Architecture & Patterns

### Directory Structure & Separation of Concerns
- **`src/`**: Contains application logic and scene implementations.
    - **`main.cpp`**: Handles GLFW initialization, input loop, and main render loop. Avoid adding heavy scene logic here.
    - **`resources/`**: Utility implementations (e.g., proper texture loading wrappers in `texture.hpp/cpp`).
    - **`scene/`**: Contains scene logic and components.
        - **`CityScene`**: High-level scene composition (lights, camera, object instantiation).
        - **`SimpleMesh`**: Reusable class for managing geometry (VAOs/VBOs) and layout. Use this for adding new objects.
        - **`Skybox`**: Encapsulates skybox rendering logic.
- **`include/`**: Reusable utility classes (e.g., `Shader`, `Camera`).
- **`shader/`**: GLSL source files. Maintained separately from C++ code.
- **`resource/`**: Assets like textures and HDRIs.

### Resource Loading Pattern
- **Texture Loading**: Use `src/resources/texture.hpp`. It wraps `stb_image` and handles OpenGL texture parameters and mipmaps.
    - **Critical**: `STB_IMAGE_IMPLEMENTATION` is defined in `src/resources/texture.cpp`. **DO NOT** define it again in any other file to avoid linker errors.
- **Path Resolution**: Use `std::filesystem::path` for all file I/O.
- **Macros**: Do not hardcode absolute paths. Use CMake-injected preprocessor definitions:
    - `SHADER_DIR`: Absolute path to `shader/` directory.
    - `TEXTURE_DIR`: Absolute path to `resource/` directory.
- **Example**:
    ```cpp
    std::string path = (std::filesystem::path(TEXTURE_DIR) / "my_texture.png").string();
    ```

### Rendering Loop
1. **Init**: Initialize GLFW, GLAD, and ImGui. Load `valid` Scene objects (e.g., `CityScene`).
2. **Update**: Calculate `deltaTime` and process input.
3. **Render**:
    - Clear buffers (`glClear`).
    - Bind standard shaders.
    - Call `scene.renderScene(...)`.
    - Render skybox `scene.renderSkybox(...)` last (optimization).
    - Render UI (ImGui) on top.
4. **Swap**: `glfwSwapBuffers`.

### Modern OpenGL Practices
- **Strict Core Profile**: Do not use immediate mode (`glBegin`/`glEnd`).
- **Geometry Management**: Use the `SimpleMesh` class for creating and rendering geometry. It handles VAO/VBO creation and destruction (RAII).
- **Shader Wrapper**: Use the `Shader` class (`include/shader.hpp`) for loading and setting uniforms.
- **Math**: Use `glm` for all vector and matrix operations.

## Build & Run
- **Build System**: CMake (out-of-source build recommended).
- **Commands**:
    ```bash
    mkdir -p build && cd build
    cmake ..
    cmake --build .
    ```
- **Execution**: Run the binary from the `build` directory. The resource paths are absolute, so the working directory usually doesn't affect asset loading, but `bin/` is the standard output location.

## Coding Style
- **C++ Standard**: C++17. Use `auto`, `constexpr`, and standard smart pointers where applicable.
- **Headers**: Use `#pragma once`.
- **Logging**: Use `std::cerr` for errors and `std::cout` for info. Check for shader compilation errors explicitly.
