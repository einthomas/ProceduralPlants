#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Object3D.h"
#include "Shader.h"
#include "LSystem.h"
#include "Util.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>

static int width = 1280, height = 720;
static const char *title = "Procedural plants";

bool initGLEW();
GLFWwindow *initGLFW();

void parseProductionsBuffer(char *str, std::map<char, std::string> *map);

int main() {
    GLFWwindow* window = initGLFW();
    if (window == NULL) {
        return 0;
    }
    ImGui_ImplGlfwGL3_Init(window, true);

    // activate v-sync
    glfwSwapInterval(1);

    if (!initGLEW()) {
        return 0;
    }

    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f);
    const float FOV = glm::radians(70.0f);
    const float CAMERA_NEAR = 0.01f;
    const float CAMERA_FAR = 100.0f;
    glm::mat4 projectionMatrix = glm::perspectiveFov(
        FOV,
        static_cast<float>(width),
        static_cast<float>(height),
        CAMERA_NEAR,
        CAMERA_FAR
    );
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    LSystem lsystem;
    std::map<char, std::string> productions;
    std::vector<Object3D> object3Ds;

    glViewport(0, 0, width, height);
    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    char axiomBuffer[2] = "F";
    char productionsBuffer[256] = "F->F[\\F]F[/F][F]";
    int depth = 4;
    float stepSize = 0.075f;
    float angleIncrement = 20.0f;
    float positionX = 0.0f, positionY = -1.0f, positionZ = 0.0f;
    parseProductionsBuffer(productionsBuffer, &productions);
    object3Ds = lsystem.produceTree(
        axiomBuffer,
        productions,
        depth,
        glm::vec3(positionX, positionY, positionZ),
        stepSize,
        glm::radians(angleIncrement)
    );

    int fKeyStates[10] = { 0 };
    bool drawHelpWindow = true;
    bool drawSettingsWindow = false;
    bool drawStatsWindow = false;
    
    double time = glfwGetTime();
    double previousTime = time;
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double delta = currentTime - previousTime;
        previousTime = currentTime;

        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && fKeyStates[0] == GLFW_RELEASE) {
            drawHelpWindow = !drawHelpWindow;
        }
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && fKeyStates[1] == GLFW_RELEASE) {
            drawSettingsWindow = !drawSettingsWindow;
        }
        if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && fKeyStates[2] == GLFW_RELEASE) {
            drawStatsWindow = !drawStatsWindow;
        }

        // Help window
        if (drawHelpWindow) {
            ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Help (F1)");

            ImGui::Text("F1 - Toggle help window");
            ImGui::Text("F2 - Toggle settings window");
            ImGui::Text("F3 - Toggle stats window");

            ImGui::End();
        }

        // Settings window
        if (drawSettingsWindow) {
            ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Settings (F2)");

            if (ImGui::InputText("Axiom", axiomBuffer, 2) |
                ImGui::InputText("Productions", productionsBuffer, 256) |
                ImGui::SliderInt("Depth", &depth, 1, 10) |
                ImGui::SliderFloat("Step size", &stepSize, 0, 1) |
                ImGui::SliderFloat("Angle increment", &angleIncrement, 0, 360) |
                ImGui::SliderFloat("X", &positionX, -10, 10) |
                ImGui::SliderFloat("Y", &positionY, -10, 10) |
                ImGui::SliderFloat("Z", &positionZ, -10, 10)) {
                parseProductionsBuffer(productionsBuffer, &productions);
                object3Ds = lsystem.produceTree(
                    axiomBuffer,
                    productions,
                    depth,
                    glm::vec3(positionX, positionY, positionZ),
                    stepSize,
                    glm::radians(angleIncrement)
                );
            }

            ImGui::End();
        }

        // Stats window
        if (drawStatsWindow) {
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Stats (F3)");

            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

            ImGui::End();
        }

        for (int i = 0; i < 10; i++) {
            fKeyStates[i] = glfwGetKey(window, GLFW_KEY_F1 + i);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < object3Ds.size(); i++) {
            RenderInfo renderInfo(viewMatrix, projectionMatrix, cameraPosition);
            object3Ds[i].draw(shader, renderInfo);
        }

        ImGui::Render();
        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}

void parseProductionsBuffer(char *productionsArray, std::map<char, std::string> *productions) {
    productions->clear();
    std::vector<std::string> splitProductions = Util::splitString(productionsArray, ";");
    for (int i = 0; i < splitProductions.size(); i++) {
        std::vector<std::string> splitProduction = Util::splitString(splitProductions[i], "->");
        if (splitProduction.size() != 2) {
            break;
        }
        splitProduction[0] = Util::trim(splitProduction[0]);
        splitProduction[1] = Util::trim(splitProduction[1]);
        (*productions)[(splitProduction[0]).c_str()[0]] = splitProduction[1];
    }
}

bool initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}

GLFWwindow *initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    //GLFWwindow *window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), nullptr);
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}
