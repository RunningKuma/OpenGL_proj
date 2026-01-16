#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"

class SimpleMesh
{
public:
    // Layout: e.g. {3, 3, 2} means specific attribute sizes.
    // Assumes GL_FLOAT type for all.
    SimpleMesh(const std::vector<float> &vertices, const std::vector<int> &layout);
    ~SimpleMesh();

    SimpleMesh(const SimpleMesh &) = delete;
    SimpleMesh &operator=(const SimpleMesh &) = delete;

    SimpleMesh(SimpleMesh &&other) noexcept;
    SimpleMesh &operator=(SimpleMesh &&other) noexcept;

    void draw(const Shader &shader, unsigned int textureId, const glm::mat4 &model) const;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    size_t vertexCount = 0;
};
