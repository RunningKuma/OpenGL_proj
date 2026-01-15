#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"

class CityScene
{
public:
    bool init();
    void update(float dt, float timeSeconds);
    void renderScene(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void shutdown();

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int roadVAO = 0;
    unsigned int roadVBO = 0;
    unsigned int paveVAO = 0;
    unsigned int paveVBO = 0;
    unsigned int skyboxVAO = 0;
    unsigned int skyboxVBO = 0;
    unsigned int texture = 0;
    unsigned int roadTexture = 0;
    unsigned int paveTexture = 0;
    unsigned int skyboxTexture = 0;
    float spin = 0.0f;
};
