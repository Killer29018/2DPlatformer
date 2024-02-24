#include "Window.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <GL/gl.h>

#include <iostream>

#include "events/Events.hpp"

uint32_t Window::s_Count = 0;

Window::Window() {}

Window::Window(glm::ivec2 windowSize, const char* title) : m_Size(windowSize)
{
    initGLFW(title);
    attach(this);

    s_Count++;
}
Window::Window(uint32_t screenWidth, uint32_t screenHeight, const char* title)
    : m_Size({ screenWidth, screenHeight })
{
    initGLFW(title);
    attach(this);

    s_Count++;
}

Window::Window(Window&& other)
{
    m_Size = other.m_Size;
    m_Observers = std::move(other.m_Observers);
    m_Window = std::move(other.m_Window);

    other.m_Observers.clear();
    other.m_Window = nullptr;
}

Window::~Window()
{
    s_Count--;
    if (s_Count == 0)
    {
        glfwTerminate();
    }
}

Window& Window::operator=(Window&& other)
{
    m_Size = other.m_Size;
    m_Observers = std::move(other.m_Observers);
    m_Window = std::move(other.m_Window);

    other.m_Observers.clear();
    other.m_Window = nullptr;

    return *this;
}

void Window::getEvents() { glfwPollEvents(); }

GLFWwindow* Window::getWindow()
{
    if (m_Window)
        return m_Window.value();
    else
        throw window_existence_error();
}

const glm::ivec2& Window::getSize() { return m_Size; }

void Window::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::KeyboardPress:
        {
            KeyboardEvent* keyEvent = (KeyboardEvent*)event;

            if (keyEvent->action != GLFW_PRESS) return;

            switch (keyEvent->key)
            {
            case GLFW_KEY_ESCAPE:
                WindowCloseEvent wcEvent;
                notify(&wcEvent);

                shouldClose(true);
                break;
            }
            break;
        }
    case EventType::MouseEnter:
        {
            MouseEnterEvent* mouseEvent = (MouseEnterEvent*)event;
            if (mouseEvent->entered)
            {
                m_FirstMouse = true;
            }
            break;
        }
    case EventType::WindowResize:
        {
            const WindowResizeEvent* rEvent = reinterpret_cast<const WindowResizeEvent*>(event);
            m_Size = glm::vec2{ rEvent->newWidth, rEvent->newHeight };

            glViewport(0, 0, m_Size.x, m_Size.y);

            break;
        }
    default:
        break;
    }
}

void Window::setInputMode(int32_t mode, int32_t value)
{
    glfwSetInputMode(getWindow(), mode, value);
}

void Window::setWindowHint(int32_t hint, int32_t value) { glfwWindowHint(hint, value); }

void Window::initGLFW(const char* title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(getSize().x, getSize().y, title, NULL, NULL);
    if (!getWindow())
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(getWindow());

    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "OpenGL Version: " << GLAD_VERSION_MAJOR(version) << "."
              << GLAD_VERSION_MINOR(version) << "\n";

    glViewport(0, 0, getSize().x, getSize().y);

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(getWindow(), (void*)this);

    glfwSetKeyCallback(getWindow(), keyboardEvent);
    glfwSetCursorPosCallback(getWindow(), mouseMoveEvent);
    glfwSetCursorEnterCallback(getWindow(), mouseEnteredEvent);
    glfwSetMouseButtonCallback(getWindow(), mousePressEvent);
    glfwSetWindowSizeCallback(getWindow(), windowResizeEvent);
}

void Window::keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    KeyboardEvent event;
    event.key = key;
    event.action = scancode;
    event.action = action;
    event.mods = mods;

    if (!ImGui::GetIO().WantCaptureKeyboard) w->notify(&event);
}

void Window::mouseMoveEvent(GLFWwindow* window, double xPos, double yPos)
{
    static float previousX = 0.0f;
    static float previousY = 0.0f;

    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    if (w->m_FirstMouse)
    {
        previousX = xPos;
        previousY = yPos;
        w->m_FirstMouse = false;
    }
    double xOffset = xPos - previousX;
    double yOffset = previousY - yPos;

    previousX = xPos;
    previousY = yPos;

    {
        MouseMoveEvent moveEvent;
        moveEvent.xOffset = xOffset;
        moveEvent.yOffset = yOffset;

        moveEvent.xPosition = xPos;
        moveEvent.yPosition = yPos;

        moveEvent.xPercent = xPos / w->getSize().x;
        moveEvent.yPercent = yPos / w->getSize().y;

        w->notify(&moveEvent);
    }

    if (w->m_LeftMouseClicked || w->m_RightMouseClicked)
    {
        MouseDraggedEvent mdEvent;
        mdEvent.xPosition = xPos;
        mdEvent.yPosition = yPos;
        mdEvent.xOffset = xOffset;
        mdEvent.yOffset = yOffset;
        mdEvent.leftClick = w->m_LeftMouseClicked;
        mdEvent.rightClick = w->m_RightMouseClicked;

        if (!ImGui::GetIO().WantCaptureMouse) w->notify(&mdEvent);
    }
}

void Window::mouseEnteredEvent(GLFWwindow* window, int entered)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    MouseEnterEvent event;
    event.entered = entered;

    w->notify(&event);
}

void Window::mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    MousePressEvent mpEvent;
    mpEvent.leftClick = (button == GLFW_MOUSE_BUTTON_LEFT);
    mpEvent.rightClick = (button == GLFW_MOUSE_BUTTON_RIGHT);

    MouseReleaseEvent mrEvent;
    mrEvent.leftClick = (button == GLFW_MOUSE_BUTTON_LEFT);
    mrEvent.rightClick = (button == GLFW_MOUSE_BUTTON_RIGHT);

    bool pressed = (action == GLFW_PRESS);
    bool released = (action == GLFW_RELEASE);

    if (w->m_LeftMouseClicked)
    {
        w->m_LeftMouseClicked = !(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE);
    }
    else
    {
        w->m_LeftMouseClicked = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
    }

    if (w->m_RightMouseClicked)
    {
        w->m_RightMouseClicked = !(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE);
    }
    else
    {
        w->m_RightMouseClicked = (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
    }

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    if (pressed)
    {
        mpEvent.xPosition = xPos;
        mpEvent.yPosition = yPos;
        if (!ImGui::GetIO().WantCaptureMouse) w->notify(&mpEvent);
    }

    if (released)
    {
        mrEvent.xPosition = xPos;
        mrEvent.yPosition = yPos;
        if (!ImGui::GetIO().WantCaptureMouse) w->notify(&mrEvent);
    }
}

void Window::windowResizeEvent(GLFWwindow* window, int width, int height)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    glm::ivec2 size = w->getSize();

    WindowResizeEvent wrEvent;
    wrEvent.previousWidth = size.x;
    wrEvent.previousHeight = size.y;
    wrEvent.newWidth = width;
    wrEvent.newHeight = height;

    w->notify(&wrEvent);
}
