#pragma once

#include "Window.hpp"
#include "events/EventHandler.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
  public:
    Camera();
    Camera(Window* window, glm::vec3 position);

    ~Camera(){};

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setPosition(glm::vec3 position) { m_Position = position; }
    void setXYPosition(glm::vec2 position)
    {
        m_Position.x = position.x;
        m_Position.y = position.y;
    }

  private:
    glm::vec3 m_Position;

    Window* m_Window;

    float m_NearClipping = 0.01f;
    float m_FarClipping = 1500.0f;
};
