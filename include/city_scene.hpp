#pragma once

#include <memory> 
#include <vector>

#include "shader.hpp"
#include "model.hpp"
#include "skybox.hpp"

class CityScene
{
public:
    bool init();
    void update(float dt, float timeSeconds);
    void renderScene(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void renderSkybox(Shader &skyboxShader, const glm::mat4 &view, const glm::mat4 &projection) const;
    void shutdown();

    // Get current time of day for UI display (0-1, where 0.5 = noon)
    float getTimeOfDay() const { return timeOfDay; }
    
    // Set time scale (1.0 = real time, higher = faster)
    void setTimeScale(float scale) { timeScale = scale; }
    float getTimeScale() const { return timeScale; }


private:
    std::unique_ptr<Model> cityModel;  // CITY glTF model
    std::unique_ptr<Skybox> skybox;

    float spin = 0.0f;
    
    // Time of day system (0.0 = midnight, 0.25 = 6AM, 0.5 = noon, 0.75 = 6PM)
    float timeOfDay = 0.35f;  // Start at ~8:24 AM (morning)
    float timeScale = 0.02f;  // Time passes at 2% real speed (1 day = ~50 real minutes)
};
