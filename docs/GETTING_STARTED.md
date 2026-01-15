# OpenGL 城市场景脚手架 — 快速上手

## 构建与运行
- 依赖：CMake 3.15+、C++17 工具链、OpenGL 3.3 Core、GLFW、GLM；GLAD 已内置。
- 配置：`cmake -S . -B build`（或 VS Code 任务“CMake Configure”）。
- 编译：`cmake --build build`（或任务“CMake Build”）。
- 运行：`./build/bin/LearningOpenGL`（macOS/Linux）或 `build\bin\LearningOpenGL.exe`（Windows）；默认开启 VSync。
- 着色器：编译期定义 `SHADER_DIR=shader/`，运行时最好从仓库根目录启动，或保持该定义以免路径错误。
- VS Code 一键：打开命令面板运行配置/编译任务即可；也可在“运行和调试”视图新建 `cmake --build build && ./build/bin/LearningOpenGL` 的 Launch 配置。

## 代码结构（对应分工）
- 应用壳与相机：窗口、输入、Delta Time、FPS 相机控制位于 [src/main.cpp](../src/main.cpp) 与 [include/camera.hpp](../include/camera.hpp)。
- 着色器工具：编译/链接与 uniform 便捷函数在 [include/shader.hpp](../include/shader.hpp)。
- 场景模块（地标渲染）：[src/scene/city_scene.hpp](../src/scene/city_scene.hpp) / [src/scene/city_scene.cpp](../src/scene/city_scene.cpp) 管理 VAO/VBO 与渲染入口；把占位立方体换成真实模型即可。
- 光照/阴影：方向光、点光、聚光参数在 [src/render/lighting.hpp](../src/render/lighting.hpp)，按需塞进你的光照着色器与 uniform。
- 水面与天空盒：可调参数在 [src/effects/water.hpp](../src/effects/water.hpp)，供 GLSL 使用。
- 模型加载：Assimp 入口桩在 [src/resources/model_loader.hpp](../src/resources/model_loader.hpp)，扩展以生成顶点/索引缓冲。
- 应用上下文：共享相机、视口等跨模块信息在 [src/core/app_context.hpp](../src/core/app_context.hpp)。
- 现有着色器：`shader/` 下有 `base.vert`/`base.frag`（冒烟测试）以及旧的 `shader.vert`/`shader.frag`（光照实验）。

## 输入与相机
- WASD：XZ 平面移动；Space/Left Shift：上/下。
- 鼠标：环视（指针锁定）；滚轮：缩放视角。
- 方向键：无鼠标环境下的小幅视角微调。
- Esc：退出。

## 建议的下一步
- 在 `ModelLoader` 集成 Assimp，用真实地标替换 `CityScene` 中的占位立方体。
- 基于 `lighting.hpp` 定义的光源结构编写光照着色器，并在 `CityScene::render` 里推送对应 uniform。
- 添加水面/天空盒渲染通道，使用 `WaterSettings`/`SkyboxSettings` 并加载专用纹理或立方体贴图。
- 扩展 `AppContext` 共享时间、输入状态或全局配置给各模块。

## 跨平台提示
- macOS：已定义 `GL_SILENCE_DEPRECATION`，GLFW forward-compat 打开；从仓库根目录运行以正确找到着色器。
- Windows：确保 CMake 能找到 `glfw3`、`glm`；同样从仓库根目录运行或调整 `SHADER_DIR`。
- Linux：用包管理器安装 `glfw`/`glm` 或自行编译；着色器路径同上。

## 调试
- 黑屏时：先检查 `shader/` 的着色器是否加载成功，以及 OpenGL 3.3 Core 上下文是否创建。
- 查看 uniform/GL 错误：可插入 `glGetError()`，或用 RenderDoc / OpenGL 调试器；VSync 保持开启便于逐帧检查。
- VS Code 调试：在“运行和调试”中新建 `cppdbg` 或 `lldb` 配置，工作目录设为项目根，程序设为 `build/bin/LearningOpenGL`，可在 `src/main.cpp` 或场景模块内下断点。
- 日志：必要时在渲染循环打印关键 uniform/矩阵或加载路径；保持输出简短以免拖慢帧率。
