#pragma once

#include <glm/glm.hpp>

struct WaterSettings
{
    float waveSpeed = 0.05f;
    float waveStrength = 0.02f;
    glm::vec3 deepColor{0.0f, 0.05f, 0.08f};
    glm::vec3 shallowColor{0.05f, 0.15f, 0.2f};
};

struct SkyboxSettings
{
    float exposure = 1.0f;
    float gamma = 2.2f;
};
