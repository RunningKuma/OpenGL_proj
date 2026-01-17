// NOLINTBEGIN(readability-magic-numbers)

#include "city_scene.hpp"

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"

bool CityScene::init()
{
    const std::filesystem::path texRoot = std::filesystem::path(TEXTURE_DIR);

    // Load CITY glTF model
    std::string cityModelPath = (texRoot / "CITY" / "scene.gltf").string();
    cityModel = std::make_unique<Model>(cityModelPath);

    skybox = std::make_unique<Skybox>();
    bool skyboxLoaded = skybox->init(texRoot / "NightSkyHDRI003_1K" / "NightSkyHDRI003_4K_TONEMAPPED.jpg");

    return skyboxLoaded;
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

    // Directional light (Night/Moonlight)
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.03f, 0.03f, 0.06f);   // Very dark blueish ambient for night
    shader.setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.2f);      // Dim blueish moonlight
    shader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);     // Reduced specular

    // Street lamp point lights (warm orange/yellow color)
    // Positions along the main road in model space (scaled by 0.2)
    const std::array<glm::vec3, 8> streetLampPositions = {{
        glm::vec3(-8.0f, 3.5f, 15.0f),
        glm::vec3(-8.0f, 3.5f, 5.0f),
        glm::vec3(-8.0f, 3.5f, -5.0f),
        glm::vec3(-8.0f, 3.5f, -15.0f),
        glm::vec3(8.0f, 3.5f, 15.0f),
        glm::vec3(8.0f, 3.5f, 5.0f),
        glm::vec3(8.0f, 3.5f, -5.0f),
        glm::vec3(8.0f, 3.5f, -15.0f),
    }};

    shader.setInt("numPointLights", static_cast<int>(streetLampPositions.size()));

    for (size_t i = 0; i < streetLampPositions.size(); ++i)
    {
        std::string prefix = "pointLights[" + std::to_string(i) + "]";
        shader.setVec3(prefix + ".position", streetLampPositions[i]);
        // Warm street lamp color (orange-yellow)
        shader.setVec3(prefix + ".ambient", 0.1f, 0.07f, 0.02f);
        shader.setVec3(prefix + ".diffuse", 1.0f, 0.7f, 0.3f);    // Warm orange
        shader.setVec3(prefix + ".specular", 1.0f, 0.8f, 0.5f);
        // Attenuation for ~50 unit range
        shader.setFloat(prefix + ".constant", 1.0f);
        shader.setFloat(prefix + ".linear", 0.09f);
        shader.setFloat(prefix + ".quadratic", 0.032f);
    }

    glm::mat4 invView = glm::inverse(view);
    shader.setVec3("viewPos", glm::vec3(invView[3]));

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw CITY model
    if (cityModel)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        // City model - grand cityscape
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Fix orientation (Z-up to Y-up)
        model = glm::scale(model, glm::vec3(0.2f));  // Large scale for grand cityscape
        shader.setMat4("model", model);
        cityModel->Draw(shader);
    }
}

void CityScene::renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const
{
    if (skybox)
    {
        skybox->draw(skyboxShader, view, projection);
    }
}

void CityScene::shutdown() // NOLINT(readability-make-member-function-const)
{
    if (skybox)
        skybox->shutdown();

    // Model and Mesh unique_ptrs clean up themselves via destructors
}

// NOLINTEND(readability-magic-numbers)
