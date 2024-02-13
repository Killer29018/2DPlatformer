#include "Camera.hpp"

#include "Events.hpp"

Camera::Camera() : m_WindowSize({ 0, 0 }), m_Position({ 0.0f, 0.0f, 0.0f }) {}

Camera::Camera(glm::ivec2 windowSize, glm::vec3 position)
    : m_WindowSize(windowSize), m_Position(position)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_NearClipping, m_FarClipping);
}
