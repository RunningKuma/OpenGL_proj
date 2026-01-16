#pragma once

#include <memory>

#include "shader.hpp"
#include "simple_mesh.hpp"
#include "skybox.hpp"

class CityScene
{
public:
    bool init();
    void update(float dt, float timeSeconds);
    void renderScene(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void shutdown();

private:
    std::unique_ptr<SimpleMesh> cubeMesh;
    std::unique_ptr<SimpleMesh> roadMesh;
    std::unique_ptr<SimpleMesh> paveMesh;

    std::unique_ptr<Skybox> skybox;

    unsigned int cubeTexture = 0;
    unsigned int roadTexture = 0;
    unsigned int paveTexture = 0;

    float spin = 0.0f;
};
