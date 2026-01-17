# OpenGL City Scene 项目开发指南

## 1. 项目概览
本项目是一个基于 C++17 和 OpenGL 3.3 Core Profile 的 3D 城市场景渲染程序。
- **构建系统**: CMake
- **核心库**: GLFW (窗口), GLAD (加载器), GLM (数学), stb_image (图像), ImGui (界面)

## 2. 目录结构
```text
src/
├── main.cpp                // 程序入口 (窗口初始化, 主循环)
├── scene/
│   ├── city_scene.cpp      // 主要场景逻辑 (物体实例化, 渲染调用)
│   ├── simple_mesh.cpp     // 网格封装 (VAO/VBO 管理)
│   └── skybox.cpp          // 天空盒实现
├── resources/
│   ├── texture.cpp         // 纹理加载工具
│   └── model_loader.hpp    // OBJ 模型加载器
└── ...
shader/                     // GLSL 着色器文件
resource/                   // 资产 (纹理, 模型, HDRI)
```

## 3. 快速上手：如何导入新模型

本项目内置了一个轻量级的 OBJ 模型加载器，支持读取 OBJ 几何体以及 MTL 材质文件的颜色信息（自动生成纹理调色板）。

### 第一步：准备资源
将你的 `.obj` 和 `.mtl` 文件放入 `resource/` 目录下的某个子文件夹中。
例如：`resource/my_building/building.obj`

### 第二步：在 CityScene 中声明
打开 `src/scene/city_scene.hpp`，在 `CityScene` 类中添加一个 `SceneModel` 成员变量。

```cpp
// src/scene/city_scene.hpp
private:
    SceneModel myNewBuilding; // 新增这一行
```

### 第三步：加载模型
打开 `src/scene/city_scene.cpp`，在 `init()` 函数中调用 `loadModel`。

```cpp
// src/scene/city_scene.cpp - init()
bool CityScene::init() {
    // ... 其他初始化代码 ...
    
    // 使用 loadModel 辅助函数 (自动处理 obj 解析和纹理生成)
    const std::filesystem::path texRoot = std::filesystem::path(TEXTURE_DIR);
    myNewBuilding = loadModel(texRoot / "my_building" / "building.obj");

    // ...
}
```

### 第四步：渲染模型
在 `renderScene()` 函数中添加绘制逻辑。记得处理模型矩阵（位置、旋转、缩放）。

```cpp
// src/scene/city_scene.cpp - renderScene(...)
void CityScene::renderScene(Shader &shader, ...) {
    // ... 其他渲染代码 ...

    // 绘制新建筑
    if (myNewBuilding.mesh) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. 位移
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); 
        // 2. 缩放 (如果模型太大或太小)
        model = glm::scale(model, glm::vec3(0.1f)); 
        // 3. 旋转 (例如修正 OBJ 的 Z-up 坐标系到 OpenGL 的 Y-up)
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // 绑定材质并绘制
        // 如果模型没有自己的纹理 (textureId == 0)，可以使用一个默认纹理 (如 cubeTexture)
        unsigned int texToUse = myNewBuilding.textureId != 0 ? myNewBuilding.textureId : cubeTexture;
        
        // 实际上这行代码会处理 shader uniform 设置和 draw call
        myNewBuilding.mesh->draw(shader, texToUse, model);
    }
}
```

### 第五步：清理资源
在 `shutdown()` 中清理纹理。如果不手动清理，GPU 内存会在程序结束时释放，但为了规范，建议添加：

```cpp
// src/scene/city_scene.cpp - shutdown()
void CityScene::shutdown() {
    // ...
    glDeleteTextures(1, &myNewBuilding.textureId);
}
```

## 4. 核心类说明

### SimpleMesh
- 负责管理 VAO 和 VBO。
- 支持自定义顶点布局。目前标准布局为 `{3, 3, 2}` (Position, Normal, UV)。
- **使用方式**: `mesh->draw(shader, textureID, modelMatrix);`

### ModelLoader
- 位于 `src/resources/model_loader.hpp`。
- `loadOBJ(path)`:
    - 解析 OBJ 顶点位置及面信息。
    - 自动计算面法线（Flat Normals）。
    - 解析关联的 MTL 文件中的 `Kd` (漫反射) 颜色。
    - 生成一个 1xN 像素的调色板对应的纹理数据。
    - 自动为顶点分配 UV 坐标以映射到调色板颜色。

### Texture 工具
- `loadTexture2D`: 加载普通图片 (.png, .jpg)。
- `createTextureFromData`: 从内存数据创建纹理（用于 MTL 颜色调色板）。
