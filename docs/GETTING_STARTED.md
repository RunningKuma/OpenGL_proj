# OpenGL 项目快速上手手册

这份文档将引导您从零开始配置环境、构建并运行本项目。本项目是一个 C++17 的 Modern OpenGL 应用程序，使用了 CMake 作为构建系统。

## 📋 1. 环境准备

在开始之前，请确保您的系统安装了以下工具。

### Windows 用户
1.  **C++ 编译器**: 推荐安装 [MinGW-w64](https://www.msys2.org/) 或安装 Visual Studio (MSVC)。
2.  **CMake**: 下载并安装 [CMake](https://cmake.org/download/) (请确保添加到系统 PATH 环境变量)。
3.  **Visual Studio Code**: 推荐的编辑器。
    *   安装插件: **C/C++** (Microsoft), **CMake Tools** (Microsoft)。

### macOS / Linux 用户
*   **macOS**: 安装 Xcode Command Line Tools (`xcode-select --install`) 和 CMake (`brew install cmake`).
*   **Linux**: 安装 GCC, Make, 和 CMake (`sudo apt-get install build-essential cmake libgl1-mesa-dev xorg-dev`).

---

## 🏗 2. 编译与构建

本项目依赖的第三方库（GLFW, GLM, Assimp, ImGui）会在 CMake 配置阶段通过 `FetchContent` 自动下载，您**无需**手动下载这些库。

### 方法 A: 使用 VS Code 任务 (推荐)

本项目已配置好 VS Code 的自动化任务 (`.vscode/tasks.json` 不存在时可直接使用默认识别的 CMake 任务，但建议检查项目根目录是否有 `.vscode` 文件夹，若无则按以下通用步骤操作)。

1.  用 VS Code 打开 `OpenGL_proj` 文件夹。
2.  按下 `Ctrl + Shift + P` 打开命令面板。
3.  输入 **"Tasks: Run Task"** 并回车。
4.  选择 **"CMake Configure"**。
    *   *这一步会下载依赖并生成构建文件，首次运行可能需要几分钟。*
5.  配置完成后，再次运行任务，选择 **"CMake Build"**。
    *   *这一步将编译源代码并生成可执行文件。*

### 方法 B: 使用命令行

如果您更喜欢使用终端：

```bash
# 1. 确保在项目根目录下
cd /path/to/OpenGL_proj

# 2. 生成构建文件 (配置阶段)
# -S . 表示源码在当面目录
# -B build 表示构建文件生成在 build 目录
# -DCMAKE_BUILD_TYPE=Debug 设置为调试模式
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# 3. 编译项目 (构建阶段)
# --target LearningOpenGL 指定编译目标
# -j 6 使用 6 个线程并行编译
cmake --build build --config Debug --target LearningOpenGL -j 6
```

---

## ▶ 3. 运行项目

编译成功后，可执行文件位于 `build/bin/` 目录下。

### Windows
```powershell
.\build\bin\LearningOpenGL.exe
```

### macOS / Linux
```bash
./build/bin/LearningOpenGL
```

> **注意**: 请务必在项目根目录下运行命令，或者确保可执行文件能正确找到 `resource/` 和 `shader/` 目录。项目使用了 CMake 定义的绝对路径宏，通常可以在任何位置运行，但保持在根目录是好习惯。

---

## 🎮 4. 操作控制指南

进入程序后，您可以自由漫游并控制场景光照。

### ⌨ 键盘与鼠标

| 按键 | 动作 | 说明 |
| :--- | :--- | :--- |
| **W, A, S, D** | 移动 | 标准 FPS 移动方式 |
| **Space** | 上升 | 垂直向上移动 |
| **Shift** | 下降 | 垂直向下移动 |
| **鼠标移动** | 视角 | 改变观看方向 (仅在面板隐藏时有效) |
| **鼠标滚轮** | 缩放 | 调整视野 (FOV) |
| **P** | **切换面板/鼠标** | **重要**: 显示/隐藏设置面板，并释放/锁定鼠标光标 |
| **G** | 手电筒 | 开关相机前方的聚光灯 |
| **ESC** | 暂停 | 暂停程序运行 |

### 🎛 ImGui 控制面板 (按 'P' 呼出)

屏幕左上角会显示调试面板：
- **Statistics**: 显示当前 FPS 和帧生成时间。
- **Camera Info**: 显示相机坐标 (x, y, z) 和朝向 (Yaw, Pitch)。
- **Moon Light**:
    - `Use Moon`: 开关月光。
    - `Arc Angle`: 拖动滑块改变月亮在天空的位置（模拟日/月升落）。
- **Street Lamps**:
    - `L1-L4`: 控制左侧路灯开关。
    - `R1-R4`: 控制右侧路灯开关。
- **Flashlight**: 手电筒的手动开关。

---

## 🧩 5. 项目架构概览

如果您想修改代码，了解以下结构会很有帮助：

*   **`src/main.cpp`**: 程序的入口。负责初始化窗口、处理输入循环，以及调用场景渲染。
*   **`src/scene/city_scene.cpp`**: 核心场景逻辑。负责加载城市模型、设置灯光（月光、路灯）以及渲染流程。
*   **`shader/`**: 包含所有 GLSL 着色器。
    *   `shader.vert/frag`: 主着色器，处理光照计算。
    *   `skybox.vert/frag`: 天空盒渲染。
*   **`Resources`**:
    *   模型加载使用 `Assimp`，相关代码在 `src/resources/model.cpp`。
    *   纹理加载使用 `stb_image`，相关代码在 `src/resources/texture.cpp`。

## ❓ 常见问题 (FAQ)

**Q: 运行报错 "Failed to init GLFW" 或窗口一闪而过？**
*   A: 检查显卡驱动是否支持 OpenGL 3.3。尝试更新显卡驱动。

**Q: 模型全黑或没有贴图？**
*   A: 检查 `resource` 文件夹是否存在且路径正确。CMake 会自动注入资源路径，但如果移动了 `resource` 文件夹会导致加载失败。

**Q: 编译时下载很慢？**
*   A: `cmake` 使用 GitHub 镜像下载依赖库（GLFW, GLM 等）。请确保网络通畅，或手动配置代理。

---

🎉 祝您探索愉快！
