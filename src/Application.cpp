#include "Application.hpp"

#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include <GL/gl.h>

#include "events/Events.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <numeric>

Application::Application(glm::ivec2 windowSize, const char* title)
    : m_Window(Window(windowSize, title))
{
}

Application::~Application() {}

void Application::start()
{
    initialize();

    mainLoop();
}

void Application::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::ImGuiRender:
        {
            if (ImGui::Begin("Data"))
            {
            }
            ImGui::End();

            if (ImGui::Begin("Info"))
            {
                const uint32_t capacity = 40;
                static std::vector<double> previousTimes(capacity);
                static size_t index = 0;

                size_t value = std::accumulate(previousTimes.begin(), previousTimes.end(), 0);
                value /= previousTimes.size();
                previousTimes[index++ % capacity] = 1.0f / m_PreviousDT;

                ImGui::Text("FPS: %ld", value);
            }
            ImGui::End();

            TimerManager::renderImGui();

            break;
        }
    default:
        break;
    }
}

void Application::initialize()
{
    m_Camera = Camera{
        &m_Window, {0.0f, 0.0f, 1000.0f}
    };

    m_Player = { &m_Camera, &m_TileManager };
    m_Player.init();

    m_Shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    m_ImGuiManager = ImGuiManager(&m_Window);
    m_MapManager = MapManager(&m_Player, &m_TileManager, &m_Window);

    m_Window.attach(this);
    m_Window.attach(&m_Player);
    m_Window.attach(&m_MapManager);
    m_Window.attach(&m_ImGuiManager);

    attach(&m_Player);
    attach(&m_TileManager);
    attach(&m_MapManager);
    attach(&m_ImGuiManager);

    m_ImGuiManager.attach(this);
    m_ImGuiManager.attach(&m_TileManager);
    m_ImGuiManager.attach(&m_Player);
    m_ImGuiManager.attach(&m_MapManager);
    m_ImGuiManager.attach(&m_ImGuiManager);
    m_ImGuiManager.attach(&m_SaveManager);

    m_SaveManager.attach(&m_TileManager);
    m_SaveManager.attach(&m_Player);

    m_TileManager.generateMap();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_SaveManager.loadState("default");
}

void Application::mainLoop()
{
    float previousTime = glfwGetTime();
    float currentTime;

    while (m_Window.isActive())
    {
        TimerManager::startTimer("Frame Time");
        currentTime = glfwGetTime();
        float dt = currentTime - previousTime;
        previousTime = currentTime;
        m_PreviousDT = dt;

        UpdateEvent event;
        event.dt = dt;
        TimerManager::startTimer("Update");
        notify(&event);
        TimerManager::endTimer("Update");

        m_Window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ImGuiManager.preRender();

        RenderEvent render;
        render.camera = &m_Camera;
        TimerManager::startTimer("Render");
        notify(&render);
        TimerManager::endTimer("Render");

        m_ImGuiManager.postRender();

        m_Window.swapBuffers();
        TimerManager::endTimer("Frame Time");
    }
}
