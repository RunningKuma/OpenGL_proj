#include "simple_mesh.hpp"

#include <numeric>

SimpleMesh::SimpleMesh(const std::vector<float> &vertices, const std::vector<int> &layout)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    // Calculate stride
    int componentCount = std::accumulate(layout.begin(), layout.end(), 0);
    int stride = componentCount * static_cast<int>(sizeof(float));

    // Setup attributes
    uint64_t offset = 0;
    for (size_t i = 0; i < layout.size(); ++i)
    {
        glEnableVertexAttribArray(static_cast<GLuint>(i));
        glVertexAttribPointer(static_cast<GLuint>(i), layout[i], GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offset * sizeof(float)));
        offset += layout[i];
    }

    vertexCount = vertices.size() / componentCount;
    glBindVertexArray(0);
}

SimpleMesh::~SimpleMesh()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

SimpleMesh::SimpleMesh(SimpleMesh &&other) noexcept
    : VAO(other.VAO), VBO(other.VBO), vertexCount(other.vertexCount)
{
    other.VAO = 0;
    other.VBO = 0;
    other.vertexCount = 0;
}

SimpleMesh &SimpleMesh::operator=(SimpleMesh &&other) noexcept
{
    if (this != &other)
    {
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
        VAO = other.VAO;
        VBO = other.VBO;
        vertexCount = other.vertexCount;

        other.VAO = 0;
        other.VBO = 0;
        other.vertexCount = 0;
    }
    return *this;
}

void SimpleMesh::draw(const Shader &shader, unsigned int textureId, const glm::mat4 &model) const
{
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
    glBindVertexArray(0);
}
