#include "Player.hpp"
#include "Events.hpp"
#include "GLFW/glfw3.h"
#include "TileManager.hpp"
#include "glm/ext/scalar_constants.hpp"

Player::Player() : m_Camera(nullptr) {}

Player::Player(Camera* camera, TileManager* tiles) : m_Camera(camera), m_Tiles(tiles)
{
    m_Position = glm::vec3(0.5, 0.1, 1);
    m_Vel = glm::vec3(0, 0, 0);
    m_Acc = glm::vec3(0, 0, 0);

    m_Tile = Tile({ 0.0f, 0.0f }, { 1, 2 });
    m_Shader.compileFromPath("res/shaders/tile.vert.glsl", "res/shaders/tile.frag.glsl");
}

void Player::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::KeyboardPress:
        {
            KeyboardEvent* keyEvent = (KeyboardEvent*)event;

            if (keyEvent->action == GLFW_PRESS)
            {
                m_PressedKeys[keyEvent->key] = true;
            }
            else if (keyEvent->action == GLFW_RELEASE)
            {
                m_PressedKeys[keyEvent->key] = false;
            }

            break;
        }
    case EventType::Update:
        {
            UpdateEvent* updateEvent = (UpdateEvent*)event;

            m_Acc *= 0.3;

            for (auto it = m_PressedKeys.begin(); it != m_PressedKeys.end(); it++)
            {
                if (it->second)
                {
                    switch (it->first)
                    {
                    case GLFW_KEY_SPACE:
                    case GLFW_KEY_W:
                        if (m_OnGround)
                        {
                            m_OnGround = false;
                            m_Vel += m_worldUp * m_JumpSpeed;
                        }
                        break;
                    case GLFW_KEY_A:
                        m_Acc += m_worldRight * m_MovementSpeed;
                        break;
                    case GLFW_KEY_D:
                        m_Acc -= m_worldRight * m_MovementSpeed;
                        break;
                    }
                }
            }
            m_Acc += m_Gravity;

            glm::vec3 previousPosition = m_Position;

            m_Vel += m_Acc * updateEvent->dt;
            m_Vel *= m_Damping;

            glm::vec4 positionVelocity = m_Tiles->checkCollision(
                m_Position, glm::vec2{ Tile::s_TileSize, Tile::s_TileSize * 2 },
                m_Vel * updateEvent->dt);

            if (m_Position.y == positionVelocity.y)
            {
                m_OnGround = true;
            }

            m_Position = glm::vec3{ positionVelocity.x, positionVelocity.y, m_Position.z };

            m_Vel.x = positionVelocity.z / updateEvent->dt;
            m_Vel.y = positionVelocity.w / updateEvent->dt;

            m_Camera->setPosition(m_Position);
            m_Tile.setWorldPosition(m_Position);

            break;
        }
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            m_Shader.setMat4("u_view", renderEvent->camera->getViewMatrix());
            m_Shader.setMat4("u_projection", renderEvent->camera->getProjectionMatrix());

            m_Tile.render(m_Shader);

            break;
        }
    default:
        break;
    }
}
