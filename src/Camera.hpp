#pragma once

#include "EventHandler.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

class Camera : public EventObserver
{
  public:
    Camera();
    Camera(glm::ivec2 windowSize, glm::vec3 position);

    ~Camera(){};

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void receiveEvent(const Event* event) override;

    void setPosition(glm::vec3 position) { m_Position = position; }
    void setXYPosition(glm::vec2 position)
    {
        m_Position.x = position.x;
        m_Position.y = position.y;
    }

  private:
    glm::ivec2 m_WindowSize;

    glm::vec3 m_Position;

    float m_NearClipping = 0.01f;
    float m_FarClipping = 1500.0f;
};
