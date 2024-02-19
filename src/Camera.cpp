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
    float aspectRatio = (float)m_WindowSize.x / (float)m_WindowSize.y;
    float halfHeight = 1.f;
    float halfWidth = halfHeight * aspectRatio;
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_NearClipping,
                      m_FarClipping);
}

void Camera::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::WindowResize:
        {
            const WindowResizeEvent* rEvent = reinterpret_cast<const WindowResizeEvent*>(event);

            m_WindowSize = glm::ivec2{ rEvent->newWidth, rEvent->newHeight };

            break;
        }
    default:
        break;
    }
}
