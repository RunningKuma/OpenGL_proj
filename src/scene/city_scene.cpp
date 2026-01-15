// NOLINTBEGIN(readability-magic-numbers)

#include "city_scene.hpp"

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
    unsigned int loadTexture2D(const std::filesystem::path &path)
    {
        unsigned int texId = 0;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            std::cerr << "Failed to load texture: " << path << '\n';
            return 0;
        }

        const GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return texId;
    }

    unsigned int loadSkyboxTexture(const std::filesystem::path &path)
    {
        unsigned int texId = 0;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            std::cerr << "Failed to load skybox texture: " << path << '\n';
            return 0;
        }

        const GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Disable mipmaps for skybox to prevent seam artifacts
        // glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return texId;
    }
} // namespace

bool CityScene::init()
{
    const std::array<float, 180> cubeVertices = {
        // positions          // texcoords
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,

        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,

        -0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,

        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,

        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
    };

    // Road plane: Width 10 (-5 to 5), Length 100 (-50 to 50).
    const std::array<float, 48> roadVertices = {
        // positions           // normals           // texcoords
        -5.0f,
        0.0f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        10.0f,
        5.0f,
        0.0f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        10.0f,
        5.0f,
        0.0f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        -5.0f,
        0.0f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        10.0f,
        5.0f,
        0.0f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        -5.0f,
        0.0f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
    };

    // Pavement plane: Larger, Width 50 (-25 to 25), Length 100.
    const std::array<float, 48> paveVertices = {
        // positions            // normals           // texcoords
        -25.0f,
        -0.01f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        10.0f,
        25.0f,
        -0.01f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        5.0f,
        10.0f,
        25.0f,
        -0.01f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        5.0f,
        0.0f,

        -25.0f,
        -0.01f,
        50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        10.0f,
        25.0f,
        -0.01f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        5.0f,
        0.0f,
        -25.0f,
        -0.01f,
        -50.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
    };

    const std::array<float, 108> skyboxVertices = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(cubeVertices.size() * sizeof(float)), cubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Setup Road VAO (Pos3, Norm3, UV2)
    glGenVertexArrays(1, &roadVAO);
    glGenBuffers(1, &roadVBO);
    glBindVertexArray(roadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(roadVertices.size() * sizeof(float)), roadVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup Pave VAO (Pos3, Norm3, UV2)
    glGenVertexArrays(1, &paveVAO);
    glGenBuffers(1, &paveVBO);
    glBindVertexArray(paveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, paveVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(paveVertices.size() * sizeof(float)), paveVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup Skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(skyboxVertices.size() * sizeof(float)), skyboxVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    const std::filesystem::path texRoot = std::filesystem::path(TEXTURE_DIR);
    texture = loadTexture2D(texRoot / "diffuseMap.png");
    // Ensure correct relative paths
    roadTexture = loadTexture2D(texRoot / "Road007_1K-PNG" / "Road007_1K-PNG_Color.png");
    paveTexture = loadTexture2D(texRoot / "PavingStones144_1K-PNG" / "PavingStones144_1K-PNG_Color.png");
    skyboxTexture = loadSkyboxTexture(texRoot / "NightSkyHDRI003_1K" / "NightSkyHDRI003_4K_TONEMAPPED.jpg");

    return texture != 0 && roadTexture != 0 && paveTexture != 0 && skyboxTexture != 0;
}

void CityScene::update(float dt, float /*timeSeconds*/)
{
    spin += dt * 20.0f;
    if (spin > 360.0f)
    {
        spin = std::fmod(spin, 360.0f);
    }
}

void CityScene::renderScene(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const
{
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 0); // Reuse diffuse for specular for now
    shader.setFloat("material.shininess", 32.0f);

    // Directional light (moon/sun)
    shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("light.ambient", 0.3f, 0.3f, 0.4f);  // Blueish ambient for night
    shader.setVec3("light.diffuse", 0.6f, 0.6f, 0.65f); // Moonlight
    shader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    glm::mat4 invView = glm::inverse(view);
    shader.setVec3("viewPos", glm::vec3(invView[3]));

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(paveVAO);
    glBindTexture(GL_TEXTURE_2D, paveTexture);
    glm::mat4 model(1.0f);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(roadVAO);
    glBindTexture(GL_TEXTURE_2D, roadTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.002f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Cube 1
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cube 2
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0.5f, -5.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cube 3
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 8.0f));
    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

void CityScene::renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const
{
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    const glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
    skyboxShader.setMat4("view", viewNoTranslation);
    skyboxShader.setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void CityScene::shutdown()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &roadVAO);
    glDeleteBuffers(1, &roadVBO);
    glDeleteVertexArrays(1, &paveVAO);
    glDeleteBuffers(1, &paveVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &roadTexture);
    glDeleteTextures(1, &paveTexture);
    glDeleteTextures(1, &skyboxTexture);
}

// NOLINTEND(readability-magic-numbers)
