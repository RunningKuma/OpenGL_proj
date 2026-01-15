#pragma once

#include <glm/glm.hpp>

class Camera;

struct AppContext
{
    Camera *camera = nullptr;
    glm::vec2 viewport{1280.0f, 720.0f};
};
