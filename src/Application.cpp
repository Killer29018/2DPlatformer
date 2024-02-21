#include "Application.hpp"
#include "EventHandler.hpp"
#include "Events.hpp"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include <memory>

#include <GL/gl.h>
#include <algorithm>
#include <format>

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
            if (ImGui::Begin("Info"))
            {
                ImGui::Text("Frametime (ms): %.3f", m_PreviousDT);
                ImGui::Text("FPS: %d", (int)(1.0f / m_PreviousDT));

                ImGui::End();
            }

            break;
        }
    case EventType::WindowClose:
        {
            m_SaveManager.saveGame();
            break;
        }
    default:
        break;
    }
}

void Application::initialize()
{
    m_Camera = Camera{
        m_Window.getSize(), {0.0f, 0.0f, 1000.0f}
    };

    m_Player = Player(&m_Camera, &m_TileManager);

    m_Shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    m_ImGuiManager = ImGuiManager(&m_Window);
    m_MapManager = MapManager(&m_Player, &m_TileManager, &m_Window);

    m_Window.attach(this);
    m_Window.attach(&m_Camera);
    m_Window.attach(&m_Player);
    m_Window.attach(&m_MapManager);

    attach(&m_Player);
    attach(&m_TileManager);
    attach(&m_MapManager);
    attach(&m_ImGuiManager);

    m_ImGuiManager.attach(this);
    m_ImGuiManager.attach(&m_TileManager);
    m_ImGuiManager.attach(&m_Player);
    m_ImGuiManager.attach(&m_MapManager);

    m_SaveManager.attach(&m_Player);
    m_SaveManager.attach(&m_TileManager);

    m_TileManager.generateMap();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::mainLoop()
{
    float previousTime = glfwGetTime();
    float currentTime;

    while (m_Window.isActive())
    {
        currentTime = glfwGetTime();
        float dt = currentTime - previousTime;
        previousTime = currentTime;
        m_PreviousDT = dt;

        UpdateEvent event;
        event.dt = dt;
        notify(&event);

        m_Window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ImGuiManager.preRender();

        RenderEvent render;
        render.camera = &m_Camera;
        notify(&render);

        m_ImGuiManager.postRender();

        m_Window.swapBuffers();
    }
}
