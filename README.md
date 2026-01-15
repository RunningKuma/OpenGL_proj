# OpenGL City Scene Project

这是一个基于 Modern OpenGL (3.3+ Core Profile) 的渲染演示项目。主要场景包含一个带有纹理的城市地面、道路、以及基于高动态范围图像（HDRI）的夜景天空盒。

## 🛠 技术栈

- **语言**: C++17
- **构建系统**: CMake
- **图形库**: OpenGL 3.3 Core Profile
- **窗口管理**: GLFW
- **数学库**: GLM
- **纹理加载**: stb_image
- **函数加载器**: GLAD

## 📂 目录结构

```text
OpenGL_proj/
├── build/              # CMake 构建输出目录
├── include/            # 头文件 (Shader类, Camera类, GLAD等)
├── resource/           # 资源文件 (纹理, HDRI全景图)
├── shader/             # GLSL 着色器源文件
├── src/                # C++ 源文件
│   ├── main.cpp        # 程序入口 & 窗口循环
│   ├── glad.c          # GLAD 实现
│   └── scene/          # 场景逻辑
│       ├── city_scene.hpp  # 场景类声明
│       └── city_scene.cpp  # 场景渲染与资源加载实现
├── vendor/             # 第三方依赖库 (Imgui等)
└── CMakeLists.txt      # CMake 构建配置
```

## 🚀 快速开始

### 1. 环境准备
确保已安装以下工具：
- C++ 编译器 (GCC / Clang / MSVC)
- CMake (3.10+)
- 显卡驱动 (支持 OpenGL 3.3)

### 2. 编译构建
在项目根目录下打开终端：

```bash
# 1. 创建构建目录（如果不存在）
mkdir build
cd build

# 2. 配置项目
cmake ..

# 3. 编译
cmake --build .
```

### 3. 运行
编译成功后，可执行文件通常位于 `build/bin/` 目录下：

```bash
./bin/LearningOpenGL
```

## 🎮 操作说明

*   **W A S D**: 前后左右移动摄像机
*   **鼠标移动**: 改变视角（FPS 风格）
*   **滚轮**: 缩放 (Zoom)
*   **ESC**: 退出程序

## 💡 核心功能实现说明

### 1. 场景渲染 (`CityScene`)
场景逻辑封装在 `src/scene/city_scene.cpp` 中：
*   **init()**: 负责加载顶点数据（VAO/VBO）和纹理资源。
*   **renderScene()**: 渲染地面（Pavement）和道路（Road）。使用了 Phong 光照模型。
*   **renderSkybox()**: 渲染无缝天空盒。

### 2. 天空盒 (Skybox)
本项目的天空盒采用了 **Equirectangular Mapping (等距柱状投影)** 技术，而非传统的 6 面 Cubemap。
*   **资源**: 使用单张 `.jpg` 全景图 (`resource/NightSkyHDRI003_1K/...`)。
*   **着色器**: `shader/skybox.frag` 将 3D 向量坐标转换为 2D UV 坐标进行采样。
*   **注意事项**: 为了防止接缝处出现纹理拉伸，加载天空盒纹理时**禁用了 Mipmap 生成**。

### 3. 光照系统
使用了定向光（Directional Light）模拟夜间月光效果：
*   **Shader**: `shader/shader.frag`
*   **配置**: 在 `CityScene::renderScene` 中设置了偏蓝的环境光和冷白色的漫反射光，以营造夜晚氛围。

## ⚠️ 常见开发问题

1.  **stb_image 链接错误**:
    *   `#define STB_IMAGE_IMPLEMENTATION` 宏定义位于 `src/scene/city_scene.cpp` 中。请勿在其他文件中重复定义，否则会导致符号重定义错误。

2.  **纹理全黑**:
    *   确保 `shader` 中正确设置了 `material.diffuse` uniform (通常为 0)。
    *   确保光照参数 (`light.ambient`, `light.diffuse`) 不为黑色。

---
*Last Updated: 2026-01-15*
