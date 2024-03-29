#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <exception>
#include <optional>

#include "events/EventHandler.hpp"

class window_existence_error : public std::exception
{
  public:
    window_existence_error() {}
    const char* what() const noexcept { return "Window not created"; }
};

class Window : public EventDispatcher, public EventObserver
{
  public:
    Window();
    Window(glm::ivec2 windowSize, const char* title);
    Window(uint32_t screenWidth, uint32_t screenHeight, const char* title);

    Window(Window& other) = delete;
    Window(Window&& window);

    ~Window();

    Window& operator=(Window& other) = delete;
    Window& operator=(Window&& other);

    void getEvents();

    GLFWwindow* getWindow();
    const glm::ivec2& getSize();
    const glm::vec2 getAspectSize();

    void setInputMode(int32_t mode, int32_t value);

    void receiveEvent(const Event* event) override;

    bool isActive() { return !glfwWindowShouldClose(getWindow()); }
    void swapBuffers() { glfwSwapBuffers(getWindow()); }
    void shouldClose(bool value) { glfwSetWindowShouldClose(getWindow(), value); }

    static void setWindowHint(int32_t hint, int32_t value);

  private:
    std::optional<GLFWwindow*> m_Window;
    glm::ivec2 m_Size;

    static uint32_t s_Count;

    bool m_FirstMouse = true;

    bool m_LeftMouseClicked = false;
    bool m_RightMouseClicked = false;

  private:
    void initGLFW(const char* title);

    static void keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseMoveEvent(GLFWwindow* window, double xPos, double yPos);
    static void mouseEnteredEvent(GLFWwindow* window, int entered);
    static void mousePressEvent(GLFWwindow* window, int button, int action, int mods);
    static void windowResizeEvent(GLFWwindow* window, int width, int height);
};
