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
# include_directories(vendor/imgui) # Removed manual vendor path in favor of fetched content
# include_directories(vendor/imgui/backends)
include_directories(src)

# 查找 OpenGL 和 GLFW 包
find_package(OpenGL REQUIRED)

include(FetchContent)

# -----------------------------------------------------------------------------
# GLFW Logic: Find locally or fetch from GitHub
# -----------------------------------------------------------------------------
find_package(glfw3 3.3 QUIET)
if(glfw3_FOUND)
    message(STATUS "Found system GLFW")
    # Simplify compatibility: if the target glfw3::glfw exists but glfw doesn't, create an alias
    if(TARGET glfw3::glfw AND NOT TARGET glfw)
        add_library(glfw ALIAS glfw3::glfw)
    endif()
else()
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
    )
    FetchContent_MakeAvailable(glfw)
endif()

# -----------------------------------------------------------------------------
# GLM Logic: Find locally or fetch from GitHub
# -----------------------------------------------------------------------------
find_package(glm QUIET)
if(glm_FOUND)
    message(STATUS "Found system GLM")
    # Attempt to unify target name if necessary
    if(TARGET glm::glm AND NOT TARGET glm)
        add_library(glm ALIAS glm::glm)
    endif()
else()
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
    )
    FetchContent_MakeAvailable(glm)
endif()

# -----------------------------------------------------------------------------
# Assimp Logic: Find locally or fetch from GitHub
# -----------------------------------------------------------------------------
# 优先查找项目目录下的 vendor/assimp (预编译库位置)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vendor/assimp")

find_package(assimp QUIET)
if(assimp_FOUND)
    message(STATUS "Found Assimp: ${assimp_DIR}")
    if(TARGET assimp::assimp AND NOT TARGET assimp)
        add_library(assimp ALIAS assimp::assimp)
    endif()
else()
    message(STATUS "Assimp not found, fetching from GitHub...")
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1
    )
    # Optimize Assimp build: disable tests and tools to save time
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(assimp)

    # 针对 GCC 13+ 出现的悬空引用误报，强制关闭该警告
    if(TARGET assimp)
        target_compile_options(assimp PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wno-error=dangling-reference>)
        target_compile_options(assimp PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wno-dangling-reference>)
    endif()
endif()

# 下载并配置 ImGui
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4.zip
)
FetchContent_MakeAvailable(imgui)

# Add ImGui include directories explicitly since we are building it from source
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
set(IMGUI_HEADERS
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
)

# 添加可执行文件，包含 main.cpp 和 glad.c
add_executable(LearningOpenGL
    src/main.cpp
    src/glad.c
    src/scene/city_scene.cpp
    src/scene/mesh.cpp
    src/scene/skybox.cpp
    src/resources/texture.cpp
    src/resources/model.cpp
    ${IMGUI_SOURCES}
    ${IMGUI_HEADERS}
)
# 添加 ImGui 源文件

# 链接库
target_link_libraries(LearningOpenGL PRIVATE glfw OpenGL::GL glm::glm assimp)

if(APPLE)
    target_compile_definitions(LearningOpenGL PRIVATE GL_SILENCE_DEPRECATION)
endif()

target_compile_definitions(LearningOpenGL PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shader"
    TEXTURE_DIR="${CMAKE_SOURCE_DIR}/resource"
)
