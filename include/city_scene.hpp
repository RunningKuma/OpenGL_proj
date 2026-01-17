#pragma once

#include <memory> 
#include <vector>

#include "shader.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "skybox.hpp"

class CityScene
{
public:
    bool init();
    void update(float dt, float timeSeconds);
    void renderScene(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void shutdown();

    // Moonlight arc angle (0-180 degrees, 0=horizon east, 90=zenith, 180=horizon west)
    float getMoonArcAngle() const { return moonArcAngle; }
    void setMoonArcAngle(float angle) { moonArcAngle = glm::clamp(angle, 0.0f, 180.0f); }
    
    // Get computed moon position in world space
    glm::vec3 getMoonPosition() const;
    glm::vec3 getMoonDirection() const;
    
    // Get moonlight intensity
    float getMoonIntensity() const { return moonIntensity; }
    void setMoonIntensity(float intensity) { moonIntensity = intensity; }
    
    // Moon orbit radius (distance from scene center)
    float getMoonOrbitRadius() const { return moonOrbitRadius; }
    void setMoonOrbitRadius(float radius) { moonOrbitRadius = radius; }
    
    // Street lamp controls (8 lamps)
    bool isStreetLampEnabled(int index) const { return (index >= 0 && index < 8) ? streetLampEnabled[index] : false; }
    void setStreetLampEnabled(int index, bool enabled) { if (index >= 0 && index < 8) streetLampEnabled[index] = enabled; }
    void setAllStreetLampsEnabled(bool enabled) { for (int i = 0; i < 8; i++) streetLampEnabled[i] = enabled; }
    
    // Flashlight controls
    bool isFlashlightOn() const { return flashlightOn; }
    void setFlashlightOn(bool on) { flashlightOn = on; }
    void toggleFlashlight() { flashlightOn = !flashlightOn; }
    
    // Set flashlight position and direction (called from main with camera data)
    void setFlashlightParams(const glm::vec3& position, const glm::vec3& direction) {
        flashlightPosition = position;
        flashlightDirection = direction;
    }
    glm::vec3 getFlashlightPosition() const { return flashlightPosition; }
    glm::vec3 getFlashlightDirection() const { return flashlightDirection; }

private:
    std::unique_ptr<Model> cityModel;  // CITY glTF model
    std::unique_ptr<Mesh> groundPlane; // Ground plane mesh
    std::unique_ptr<Skybox> skybox;

    float spin = 0.0f;
    
    // Moonlight parameters - arc trajectory over the scene
    float moonArcAngle = 45.0f;    // Arc angle (0-180 deg): 0=east horizon, 90=zenith, 180=west horizon
    float moonOrbitRadius = 100.0f; // Distance from scene center
    float moonIntensity = 1.0f;    // Light intensity multiplier
    
    // Street lamp on/off states (8 lamps)
    bool streetLampEnabled[8] = {true, true, true, true, true, true, true, true};
    
    // Flashlight parameters
    bool flashlightOn = false;
    glm::vec3 flashlightPosition = glm::vec3(0.0f);
    glm::vec3 flashlightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
};
