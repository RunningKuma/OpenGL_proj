cmake_minimum_required(VERSION 3.15)
project(LearningOpenGL VERSION 1.0 LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 指定头文件目录
include_directories(include)
include_directories(vendor/imgui)
include_directories(vendor/imgui/backends)
include_directories(src)

# 查找 OpenGL 和 GLFW 包
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

# 添加可执行文件，包含 main.cpp 和 glad.c
add_executable(LearningOpenGL
    src/main.cpp
    src/glad.c
    src/scene/city_scene.cpp
    src/scene/simple_mesh.cpp
    src/scene/skybox.cpp
    src/resources/texture.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

# 链接库
target_link_libraries(LearningOpenGL PRIVATE glfw OpenGL::GL)

if(APPLE)
    target_compile_definitions(LearningOpenGL PRIVATE GL_SILENCE_DEPRECATION)
endif()

target_compile_definitions(LearningOpenGL PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shader"
    TEXTURE_DIR="${CMAKE_SOURCE_DIR}/resource"
)
