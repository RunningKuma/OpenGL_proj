#pragma once

#include <memory> 
#include <vector>

#include "shader.hpp"
#include "model.hpp"
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
    std::unique_ptr<Model> cityModel;  // CITY glTF model

    std::unique_ptr<Skybox> skybox;

    float spin = 0.0f;
};
