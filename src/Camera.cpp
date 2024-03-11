#include "Camera.hpp"

#include "events/Events.hpp"

#include <iostream>

Camera::Camera() : m_Position({ 0.0f, 0.0f, 0.0f }), m_Window(nullptr) {}

Camera::Camera(Window* window, glm::vec3 position) : m_Position(position), m_Window(window) {}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    glm::vec2 halfSize = m_Window->getAspectSize();
    return glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, m_NearClipping,
                      m_FarClipping);
}
