# OpenGL 城市夜景渲染引擎 (City Scene Renderer)

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

本项目是一个基于 Modern OpenGL (3.3+ Core Profile) 开发的 3D 城市夜景渲染演示程序。它展示了一个完整的图形渲染管线，包含多光源系统、HDRI 天空盒、模型加载以及基于 ImGui 的实时交互控制。

## ✨ 核心特性

- **现代渲染管线**：完全使用 OpenGL 3.3 Core Profile，杜绝过时的立即模式。
- **动态光照系统**：
  - 🌙 **月光（方向光）**：支持沿弧形轨迹运动，模拟月升月落。
  - 💡 **路灯（点光源）**：场景中包含 8 盏独立可控的路灯。
  - 🔦 **手电筒（聚光灯）**：第一人称视角的聚光灯，随相机移动。
- **沉浸式环境**：
  - **HDRI 天空盒**：使用等距柱状投影（Equirectangular）的高动态范围纹理。
  - **3D 城市模型**：集成了带有纹理的城市建筑与地面模型。
- **交互控制**：
  - 集成 **ImGui** 控制面板，可实时调节光照参数、相机位置。
  - 自由视角的 **FPS 漫游相机**。

## 🛠 技术栈

| 组件 | 技术选择 | 说明 |
| :--- | :--- | :--- |
| **语言标准** | C++17 | 利用现代 C++ 特性进行资源管理 |
| **构建工具** | CMake 3.15+ | 跨平台构建，自动管理依赖 |
| **图形 API** | OpenGL 3.3 | Core Profile |
| **窗口管理** | GLFW | 处理窗口创建、上下文及输入 |
| **扩展加载** | GLAD | 加载 OpenGL 函数指针 |
| **数学库** | GLM | 处理向量、矩阵运算 |
| **GUI 库** | Dear ImGui | 调试与参数控制界面 |
| **资源加载** | Assimp & stb_image | 模型与纹理加载 |

## 🚀 快速开始

本项目提供了详细的[**上手手册 (Getting Started)**](docs/GETTING_STARTED.md)，指导您完成环境配置、构建与运行。

### 简易运行步骤 (Windows/VS Code)

1.  **环境要求**：安装 C++ 编译器 (MinGW/MSVC) 和 CMake。
2.  **构建**：
    *   在 VS Code 中按下 `Ctrl+Shift+P`。
    *   输入 "Run Task" 并选择 **"CMake Build"**。
3.  **运行**：
    *   编译完成后，终端运行：`.\build\bin\LearningOpenGL.exe`

## 🕹 操作说明

| 按键/操作 | 功能 |
| :--- | :--- |
| **W / A / S / D** | 前后左右移动相机 |
| **Space / Shift** | 垂直上升 / 下降 |
| **鼠标移动** | 调整视角（需隐藏面板） |
| **P** | 切换 **控制面板/鼠标模式** |
| **G** | 开关 **手电筒** |
| **ESC** | 暂停/恢复 |

更多详细开发文档，请查阅 [`docs/`](docs/) 目录。
