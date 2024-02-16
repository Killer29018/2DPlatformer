#include "Application.hpp"
#include "EventHandler.hpp"
#include "Events.hpp"
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include <algorithm>

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

void Application::receiveEvent(const Event* event) {}

void Application::initialize()
{
    m_Camera = Camera{
        m_Window.getSize(), {0.0f, 0.0f, 10.0f}
    };
    m_Window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_Player = Player(&m_Camera, &m_Tiles);

    m_Shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    m_Window.attach(&m_Player);
    attach(&m_Player);
    attach(&m_Tiles);

    m_Tiles.generateMap();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_FRAMEBUFFER_SRGB);
}

void Application::mainLoop()
{
    float previousTime = 0.0f;
    float currentTime;

    while (m_Window.isActive())
    {
        currentTime = glfwGetTime();
        float dt = currentTime - previousTime;
        previousTime = currentTime;

        UpdateEvent event;
        event.dt = dt;
        notify(&event);

        m_Window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderEvent render;
        render.camera = &m_Camera;
        notify(&render);

        m_Window.swapBuffers();
    }
}
