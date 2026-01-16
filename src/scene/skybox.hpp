#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>
#include "shader.hpp"

class Skybox
{
public:
    bool init(const std::filesystem::path &texturePath);
    void draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void shutdown();

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int textureId = 0;
};
