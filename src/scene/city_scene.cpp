// NOLINTBEGIN(readability-magic-numbers)

#include "city_scene.hpp"

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "../resources/texture.hpp"
#include "../resources/model_loader.hpp"

CityScene::SceneModel CityScene::loadModel(const std::filesystem::path &path)
{
    SceneModel model;
    auto data = ModelLoader::loadOBJ(path.string());
    if (data.success)
    {
        model.mesh = std::make_unique<SimpleMesh>(data.vertices, std::vector<int>{3, 3, 2});
        if (!data.textureData.empty())
        {
            model.textureId = createTextureFromData(data.texWidth, data.texHeight, data.textureData.data());
        }
    }
    return model;
}

bool CityScene::init()
{
    const std::vector<float> cubeVertices = {
        // positions          // normals           // texcoords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

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
    // Cube: Pos(3), Norm(3), Tex(2) (Updated to include normals)
    cubeMesh = std::make_unique<SimpleMesh>(cubeVertices, std::vector<int>{3, 3, 2});
    // Road/Pave: Pos(3), Norm(3), Tex(2)
    roadMesh = std::make_unique<SimpleMesh>(roadVertices, std::vector<int>{3, 3, 2});
    paveMesh = std::make_unique<SimpleMesh>(paveVertices, std::vector<int>{3, 3, 2});

    const std::filesystem::path texRoot = std::filesystem::path(TEXTURE_DIR);

    // Load building using the helper
    buildingCTF = loadModel(texRoot / "source-Guangzhou CTF Finance Centre" / "tinker.obj");

    cubeTexture = loadTexture2D(texRoot / "diffuseMap.png");
    cubeSpecularTexture = loadTexture2D(texRoot / "specularMap.png");
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
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);

    // Ensure specular map is unbound for objects that don't have one
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeSpecularTexture);

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

    // Draw Building
    if (buildingCTF.mesh)
    {
        // Reset specular to empty/default if needed, or use same as cubes
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Move to the side of the road (Road is width 10, so X from -5 to 5)
        // Let's put it at X = 15
        model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));                                    // Scale it down
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate -90 degrees around X to fix orientation (Z-up to Y-up)
        buildingCTF.mesh->draw(shader, buildingCTF.textureId != 0 ? buildingCTF.textureId : cubeTexture, model);
    }
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
    glDeleteTextures(1, &cubeSpecularTexture);
    glDeleteTextures(1, &roadTexture);
    glDeleteTextures(1, &paveTexture);
    glDeleteTextures(1, &buildingCTF.textureId);
}

// NOLINTEND(readability-magic-numbers)
