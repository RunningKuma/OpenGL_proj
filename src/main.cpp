#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "camera.hpp"
#include "shader.hpp"
#include "city_scene.hpp"

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
    bool isPaused = false;
    bool showControlPanel = false;  // P key control panel
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, CityScene &cityScene);
void renderControlPanel(CityScene &cityScene, float fps);

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

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

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
        
        // Calculate FPS
        static float fpsTimer = 0.0f;
        static float fps = 0.0f;
        static int frameCount = 0;
        fpsTimer += deltaTime;
        frameCount++;
        if (fpsTimer >= 0.5f)  // Update FPS every 0.5 seconds
        {
            fps = static_cast<float>(frameCount) / fpsTimer;
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        processInput(window, cityScene);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.02f, 0.05f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!isPaused)
        {
            cityScene.update(deltaTime, currentFrame);
        }
        
        const float aspect = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        cityScene.renderScene(shader, view, projection);

        skyboxShader.use();
        cityScene.renderSkybox(skyboxShader, view, projection);

        // Render Control Panel (P key)
        if (showControlPanel)
        {
            renderControlPanel(cityScene, fps);
        }

        // Render Pause UI
        if (isPaused)
        {
            // Dim background
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f)); // Semi-transparent black
            ImGui::Begin("PauseOverlay", nullptr, 
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
                ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);
            
            auto text = "PAUSED";
            ImGui::SetWindowFontScale(3.0f);
            auto textSize = ImGui::CalcTextSize(text);
            ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - textSize.x) * 0.5f, (io.DisplaySize.y - textSize.y) * 0.5f));
            ImGui::Text("%s", text);
            
            ImGui::End();
            ImGui::PopStyleColor();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    cityScene.shutdown();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, CityScene &cityScene)
{
    // ESC key for pause
    static bool escPressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (!escPressedLastFrame)
        {
            isPaused = !isPaused;
            if (isPaused)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;
            }
        }
        escPressedLastFrame = true;
    }
    else
    {
        escPressedLastFrame = false;
    }

    // P key for control panel
    static bool pPressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (!pPressedLastFrame)
        {
            showControlPanel = !showControlPanel;
            if (showControlPanel)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;
            }
        }
        pPressedLastFrame = true;
    }
    else
    {
        pPressedLastFrame = false;
    }

    if (isPaused)
    {
        // Resume on click
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            isPaused = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
        return; // Skip camera controls if paused
    }

    // Keyboard movement still works when control panel is shown
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

    // Arrow keys nudge view angles (only when control panel is not shown)
    if (!showControlPanel)
    {
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
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
    if (isPaused || showControlPanel) return;  // No mouse look when panel is shown

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

void renderControlPanel(CityScene &cityScene, float fps)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Control Panel (P to toggle)", &showControlPanel);
    
    // Performance section
    ImGui::Text("Performance");
    ImGui::Separator();
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / fps);
    
    ImGui::Spacing();
    
    // Camera position section
    ImGui::Text("Camera Position");
    ImGui::Separator();
    glm::vec3 camPos = camera.Position;
    ImGui::Text("X: %.2f  Y: %.2f  Z: %.2f", camPos.x, camPos.y, camPos.z);
    ImGui::Text("Yaw: %.1f  Pitch: %.1f", camera.Yaw, camera.Pitch);
    
    ImGui::Spacing();
    
    // Moonlight section - Arc trajectory control
    ImGui::Text("Moon Light (Arc Trajectory)");
    ImGui::Separator();
    
    float arcAngle = cityScene.getMoonArcAngle();
    if (ImGui::SliderFloat("Arc Angle", &arcAngle, 0.0f, 180.0f, "%.1f deg"))
    {
        cityScene.setMoonArcAngle(arcAngle);
    }
    ImGui::Text("  0=East horizon, 90=Zenith, 180=West");
    
    float orbitRadius = cityScene.getMoonOrbitRadius();
    if (ImGui::SliderFloat("Orbit Radius", &orbitRadius, 50.0f, 200.0f, "%.0f"))
    {
        cityScene.setMoonOrbitRadius(orbitRadius);
    }
    
    float intensity = cityScene.getMoonIntensity();
    if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 3.0f, "%.2f"))
    {
        cityScene.setMoonIntensity(intensity);
    }
    
    // Display computed moon position
    glm::vec3 moonPos = cityScene.getMoonPosition();
    ImGui::Text("Moon Position:");
    ImGui::Text("  X: %.1f  Y: %.1f  Z: %.1f", moonPos.x, moonPos.y, moonPos.z);
    
    ImGui::Spacing();
    
    // Street Lamp Controls
    ImGui::Text("Street Lamps (Warm Light)");
    ImGui::Separator();
    
    // All on/off buttons
    if (ImGui::Button("All On"))
    {
        cityScene.setAllStreetLampsEnabled(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("All Off"))
    {
        cityScene.setAllStreetLampsEnabled(false);
    }
    
    // Individual lamp toggles (2 columns: Left side and Right side)
    ImGui::Text("Left Side:");
    for (int i = 0; i < 4; i++)
    {
        bool enabled = cityScene.isStreetLampEnabled(i);
        char label[32];
        snprintf(label, sizeof(label), "Lamp L%d", i + 1);
        if (ImGui::Checkbox(label, &enabled))
        {
            cityScene.setStreetLampEnabled(i, enabled);
        }
        if (i < 3) ImGui::SameLine();
    }
    
    ImGui::Text("Right Side:");
    for (int i = 4; i < 8; i++)
    {
        bool enabled = cityScene.isStreetLampEnabled(i);
        char label[32];
        snprintf(label, sizeof(label), "Lamp R%d", i - 3);
        if (ImGui::Checkbox(label, &enabled))
        {
            cityScene.setStreetLampEnabled(i, enabled);
        }
        if (i < 7) ImGui::SameLine();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Controls:");
    ImGui::BulletText("WASD - Move");
    ImGui::BulletText("Space/Shift - Up/Down");
    ImGui::BulletText("Mouse - Look around");
    ImGui::BulletText("P - Toggle this panel");
    ImGui::BulletText("ESC - Pause");
    
    ImGui::End();
}
