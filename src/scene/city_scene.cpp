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

    // Create ground plane (large flat quad)
    const float groundSize = 30.0f;
    std::vector<Vertex> groundVertices = {
        // positions                          normals              texcoords
        {{-groundSize, 0.0f, -groundSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ groundSize, 0.0f, -groundSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ groundSize, 0.0f,  groundSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-groundSize, 0.0f,  groundSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    };
    std::vector<unsigned int> groundIndices = {0, 1, 2, 2, 3, 0};
    
    // Create a dark gray texture for the ground (asphalt-like color)
    unsigned char groundColor[4] = {35, 35, 40, 255};  // Dark gray with slight blue tint
    unsigned int groundTexId = createTextureFromData(1, 1, groundColor);
    
    std::vector<Texture> groundTextures;
    Texture groundTex;
    groundTex.id = groundTexId;
    groundTex.type = "texture_diffuse";
    groundTex.path = "";
    groundTextures.push_back(groundTex);
    
    groundPlane = std::make_unique<Mesh>(groundVertices, groundIndices, groundTextures);

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

    // Get moonlight direction from arc trajectory
    glm::vec3 moonDir = getMoonDirection();
    
    // Directional light (Night/Moonlight) - cold blueish color
    shader.setVec3("dirLight.direction", moonDir);
    shader.setVec3("dirLight.ambient", 0.02f * moonIntensity, 0.02f * moonIntensity, 0.05f * moonIntensity);   // Very dark blueish ambient
    shader.setVec3("dirLight.diffuse", 0.15f * moonIntensity, 0.15f * moonIntensity, 0.3f * moonIntensity);    // Cold blueish moonlight
    shader.setVec3("dirLight.specular", 0.3f * moonIntensity, 0.3f * moonIntensity, 0.4f * moonIntensity);     // Cold specular

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

    // Count enabled lamps
    int enabledCount = 0;
    for (int i = 0; i < 8; i++) {
        if (streetLampEnabled[i]) enabledCount++;
    }
    shader.setInt("numPointLights", enabledCount);

    int lightIndex = 0;
    for (size_t i = 0; i < streetLampPositions.size(); ++i)
    {
        if (!streetLampEnabled[i]) continue;  // Skip disabled lamps
        
        std::string prefix = "pointLights[" + std::to_string(lightIndex) + "]";
        shader.setVec3(prefix + ".position", streetLampPositions[i]);
        // Warm street lamp color (orange-yellow)
        shader.setVec3(prefix + ".ambient", 0.1f, 0.07f, 0.02f);
        shader.setVec3(prefix + ".diffuse", 1.0f, 0.7f, 0.3f);    // Warm orange
        shader.setVec3(prefix + ".specular", 1.0f, 0.8f, 0.5f);
        // Attenuation for ~50 unit range
        shader.setFloat(prefix + ".constant", 1.0f);
        shader.setFloat(prefix + ".linear", 0.09f);
        shader.setFloat(prefix + ".quadratic", 0.032f);
        lightIndex++;
    }

    // Flashlight (spotlight from camera)
    shader.setBool("flashlightOn", flashlightOn);
    if (flashlightOn)
    {
        shader.setVec3("spotLight.position", flashlightPosition);
        shader.setVec3("spotLight.direction", flashlightDirection);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(20.0f)));      // Inner cone: larger for wider bright area
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f))); // Outer cone: much larger for soft falloff
        shader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 0.9f);   // Warm white
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.07f);   // Reduced for longer range
        shader.setFloat("spotLight.quadratic", 0.017f);
    }

    glm::mat4 invView = glm::inverse(view);
    shader.setVec3("viewPos", glm::vec3(invView[3]));

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw ground plane first
    if (groundPlane)
    {
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.1f, 0.0f));  // Slightly below city
        shader.setMat4("model", groundModel);
        groundPlane->Draw(shader);
    }

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

glm::vec3 CityScene::getMoonPosition() const
{
    // Moon travels in a semicircular arc from east (0 deg) to west (180 deg)
    // At 90 degrees, moon is directly overhead (zenith)
    float arcRad = glm::radians(moonArcAngle);
    
    // X: east-west position (positive X = east, negative X = west)
    // Y: height above ground
    // Z: kept at 0 (moon arc is in the X-Y plane, perpendicular to Z axis)
    glm::vec3 moonPos;
    moonPos.x = moonOrbitRadius * std::cos(arcRad);   // Starts at +X (east), ends at -X (west)
    moonPos.y = moonOrbitRadius * std::sin(arcRad);   // Height: 0 at horizon, max at zenith
    moonPos.z = 0.0f;  // Arc is in the X-Y plane
    
    return moonPos;
}

glm::vec3 CityScene::getMoonDirection() const
{
    // Light direction points FROM moon TO scene center (origin)
    glm::vec3 moonPos = getMoonPosition();
    return glm::normalize(-moonPos);  // Negative because direction is from light to scene
}

// NOLINTEND(readability-magic-numbers)
