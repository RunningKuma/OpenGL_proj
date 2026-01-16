// NOLINTBEGIN(readability-magic-numbers)

#include "city_scene.hpp"

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "../resources/texture.hpp"

bool CityScene::init()
{
    const std::vector<float> cubeVertices = {
        // positions          // texcoords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // Road plane: Width 10 (-5 to 5), Length 100 (-50 to 50).
    const std::vector<float> roadVertices = {
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
    const std::vector<float> paveVertices = {
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

    // Layouts
    // Cube: Pos(3), Tex(2)
    cubeMesh = std::make_unique<SimpleMesh>(cubeVertices, std::vector<int>{3, 2});
    // Road/Pave: Pos(3), Norm(3), Tex(2)
    roadMesh = std::make_unique<SimpleMesh>(roadVertices, std::vector<int>{3, 3, 2});
    paveMesh = std::make_unique<SimpleMesh>(paveVertices, std::vector<int>{3, 3, 2});

    const std::filesystem::path texRoot = std::filesystem::path(TEXTURE_DIR);

    cubeTexture = loadTexture2D(texRoot / "diffuseMap.png");
    roadTexture = loadTexture2D(texRoot / "Road007_1K-PNG" / "Road007_1K-PNG_Color.png");
    paveTexture = loadTexture2D(texRoot / "PavingStones144_1K-PNG" / "PavingStones144_1K-PNG_Color.png");

    skybox = std::make_unique<Skybox>();
    bool skyboxLoaded = skybox->init(texRoot / "NightSkyHDRI003_1K" / "NightSkyHDRI003_4K_TONEMAPPED.jpg");

    return cubeTexture != 0 && roadTexture != 0 && paveTexture != 0 && skyboxLoaded;
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
    shader.setInt("material.specular", 0);
    shader.setFloat("material.shininess", 32.0f);

    // Directional light
    shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("light.ambient", 0.3f, 0.3f, 0.4f);
    shader.setVec3("light.diffuse", 0.6f, 0.6f, 0.65f);
    shader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    glm::mat4 invView = glm::inverse(view);
    shader.setVec3("viewPos", glm::vec3(invView[3]));

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw Pavement
    glm::mat4 model(1.0f);
    paveMesh->draw(shader, paveTexture, model);

    // Draw Road
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.002f, 0.0f));
    roadMesh->draw(shader, roadTexture, model);

    // Draw Cubes
    // Cube 1
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    cubeMesh->draw(shader, cubeTexture, model);

    // Cube 2
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0.5f, -5.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cubeMesh->draw(shader, cubeTexture, model);

    // Cube 3
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 8.0f));
    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cubeMesh->draw(shader, cubeTexture, model);
}

void CityScene::renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const
{
    if (skybox)
    {
        skybox->draw(skyboxShader, view, projection);
    }
}

void CityScene::shutdown()
{
    if (skybox)
        skybox->shutdown();

    // Meshes (unique_ptr) clean up themselves via destructors

    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &roadTexture);
    glDeleteTextures(1, &paveTexture);
}

// NOLINTEND(readability-magic-numbers)
