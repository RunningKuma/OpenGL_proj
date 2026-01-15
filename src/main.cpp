#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "scene/city_scene.hpp"

#include <array>
#include <filesystem>
#include <iostream>

namespace
{
    constexpr unsigned int SCR_WIDTH = 1280;
    constexpr unsigned int SCR_HEIGHT = 720;

    Camera camera({0.0f, 1.2f, 5.0f});
    float lastX = SCR_WIDTH * 0.5f;
    float lastY = SCR_HEIGHT * 0.5f;
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearningOpenGL - City Scaffold", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    const std::filesystem::path shaderRoot = std::filesystem::path(SHADER_DIR);
    const auto vertexPath = shaderRoot / "shader.vert";
    const auto fragmentPath = shaderRoot / "shader.frag";
    Shader shader(vertexPath.string().c_str(), fragmentPath.string().c_str());
    shader.use();
    shader.setInt("material.diffuse", 0);

    const auto skyboxVert = shaderRoot / "skybox.vert";
    const auto skyboxFrag = shaderRoot / "skybox.frag";
    Shader skyboxShader(skyboxVert.string().c_str(), skyboxFrag.string().c_str());
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    CityScene cityScene;
    if (!cityScene.init())
    {
        std::cerr << "Failed to initialize city scene" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.02f, 0.05f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cityScene.update(deltaTime, currentFrame);
        const float aspect = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        cityScene.renderScene(shader, view, projection);

        skyboxShader.use();
        cityScene.renderSkybox(skyboxShader, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cityScene.shutdown();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Arrow keys nudge view angles for quick testing without mouse.
    const float rotationSpeed = 40.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessMouseMovement(0.0f, rotationSpeed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessMouseMovement(0.0f, -rotationSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessMouseMovement(-rotationSpeed, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessMouseMovement(rotationSpeed, 0.0f);
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    const float xoffset = static_cast<float>(xpos) - lastX;
    const float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
