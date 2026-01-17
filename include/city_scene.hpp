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
    std::unique_ptr<Mesh> cubeMesh;
    std::unique_ptr<Mesh> roadMesh;
    std::unique_ptr<Mesh> paveMesh;

    std::unique_ptr<Model> buildingCTF; 

    std::unique_ptr<Skybox> skybox;

    unsigned int cubeTexture = 0;
    unsigned int cubeSpecularTexture = 0;
    unsigned int roadTexture = 0;
    unsigned int paveTexture = 0;

    float spin = 0.0f;
};
