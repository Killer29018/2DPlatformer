#include "Player.hpp"

#include "GLFW/glfw3.h"
#include "PlayerAnimations.hpp"
#include "glad/gl.h"
#include "glm/gtx/string_cast.hpp"

#include "Animation.hpp"
#include "events/Events.hpp"
#include <any>
#include <format>
#include <iostream>

#include <imgui.h>
#include <utility>

Player::Player() : m_Camera(nullptr) {}

Player::Player(Camera* camera, TileManager* tiles) : m_Camera(camera), m_Tiles(tiles)
{
    m_Position = glm::vec3(0.0, 0.1, 0);
    m_Vel = glm::vec3(0, 0, 0);
    m_Acc = glm::vec3(0, 0, 0);
}

Player::Player(Player&& other)
    : m_Position(other.m_Position), m_VAO(other.m_VAO), m_VBO(other.m_VBO),
      m_VertexCount(other.m_VertexCount), m_Camera(other.m_Camera), m_Tiles(other.m_Tiles)
{
    m_Shader = std::move(other.m_Shader);
    m_Animation = std::move(other.m_Animation);

    other.m_VAO = 0;
    other.m_VBO = 0;
}

Player::~Player()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

Player& Player::operator=(Player&& other)
{
    m_Camera = other.m_Camera;
    m_Position = other.m_Position;
    m_Shader = std::move(other.m_Shader);
    m_Animation = std::move(other.m_Animation);
    m_Tiles = other.m_Tiles;

    m_VAO = other.m_VAO;
    m_VBO = other.m_VBO;
    m_VertexCount = other.m_VertexCount;

    other.m_VAO = 0;
    other.m_VBO = 0;

    return *this;
}

void Player::init() { setupPlayerData(); }

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

            m_Animation.update(updateEvent->dt);

            m_Acc *= 0.3 * updateEvent->dt;

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
                        m_Acc -= m_worldRight * m_MovementSpeed;
                        break;
                    case GLFW_KEY_D:
                        m_Acc += m_worldRight * m_MovementSpeed;
                        break;

                    case GLFW_KEY_R:
                        m_Position = glm::vec3(0.0f, 0.1f, m_Position.z);
                        m_Vel = glm::vec3(0.0f, 0.0f, 0.0f);
                        m_Acc = glm::vec3(0.0f, 0.0f, 0.0f);
                        break;
                    }
                }
            }
            m_Acc += m_Gravity;

            glm::vec3 previousPosition = m_Position;

            m_Vel += m_Acc * updateEvent->dt;
            m_Vel -= m_Vel * m_Damping * updateEvent->dt;

            glm::vec4 positionVelocity = m_Tiles->checkCollision(
                m_Position, Tile::s_TileSize * m_SizeScale, m_Vel * updateEvent->dt);

            if (m_Position.y == positionVelocity.y)
            {
                m_OnGround = true;
            }
            else
            {
                m_OnGround = false;
            }

            m_Position = glm::vec3{ positionVelocity.x, positionVelocity.y, m_Position.z };

            m_Vel.x = positionVelocity.z / updateEvent->dt;
            m_Vel.y = positionVelocity.w / updateEvent->dt;

            m_Camera->setXYPosition(glm::vec2{ m_Position.x, m_Position.y });

            break;
        }
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            m_Shader.bind();

            m_Animation.bindTexture();
            Texture2D::activeTexture(GL_TEXTURE0);

            m_Shader.setMat4("u_View", renderEvent->camera->getViewMatrix());
            m_Shader.setMat4("u_Projection", renderEvent->camera->getProjectionMatrix());

            glm::mat4 model(1.0f);

            glm::vec2 sizeXY = Tile::s_TileSize * m_SizeScale;
            glm::vec3 size{ sizeXY, 1.0 };

            model = glm::translate(model, m_Position);
            model = glm::scale(model, size);

            m_Shader.setMat4("u_Model", model);

            m_Shader.setIVec2("u_Index", m_Animation.getIndex());
            m_Shader.setIVec2("u_TextureSize", m_Animation.getDimensions());
            m_Shader.setInt("u_FlipTextureX", !m_FacingRight);

            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);

            break;
        }
    case EventType::ImGuiRender:
        {
            if (ImGui::Begin("Player"))
            {
                ImGui::Text("Pos: (%.3f, %.3f)", m_Position.x, m_Position.y);
                ImGui::Text("Vel: (%.3f, %.3f)", m_Vel.x, m_Vel.y);
                ImGui::Text("Acc: (%.3f, %.3f)", m_Acc.x, m_Acc.y);

                if (ImGui::Button("Reset"))
                {
                    m_Position = glm::vec3(0.0f, 0.1f, m_Position.z);

                    m_Vel = glm::vec3(0.0f, 0.0f, 0.0f);
                    m_Acc = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            ImGui::End();
            break;
        }
    case EventType::SaveGame:
        {
            const SaveGameEvent* sgEvent = reinterpret_cast<const SaveGameEvent*>(event);
            Json::Value playerRoot;

            playerRoot["position"]["x"] = m_Position.x;
            playerRoot["position"]["y"] = m_Position.y;

            (*sgEvent->root)["player"] = playerRoot;
            break;
        }
    case EventType::LoadGame:
        {
            const LoadGameEvent* lgEvent = reinterpret_cast<const LoadGameEvent*>(event);

            Json::Value playerRoot = (*lgEvent->root)["player"];
            m_Position.x = playerRoot["position"]["x"].asFloat();
            m_Position.y = playerRoot["position"]["y"].asFloat();

            break;
        }
    default:
        break;
    }
}

void Player::setupPlayerData()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    std::vector<float> vertices = {
        0.0, 0.0, 0.0f, 1.0f, //
        0.0, 1.0, 0.0f, 0.0f, //
        1.0, 1.0, 1.0f, 0.0f, //
        0.0, 0.0, 0.0f, 1.0f, //
        1.0, 1.0, 1.0f, 0.0f, //
        1.0, 0.0, 1.0f, 1.0f, //
    };

    m_VertexCount = 6;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_Shader.compileFromPath("res/shaders/player.vert.glsl", "res/shaders/player.frag.glsl");

    m_Animation = Animation<PlayerAnimations>("res/textures/PlayerTilemap.png", 32, 32,
                                              &PlayerAnimationToVec);

    m_Animation.setAnimation(PlayerAnimations::IDLE_1);

    addAnimations();
}

void Player::addAnimations()
{
    constexpr float ANIM_IDLE_TIME = 0.2f;
    constexpr float ANIM_RUN_TIME = 0.15f;
    // constexpr float ANIM_IDLE_TIME = 1.0f;
    // constexpr float ANIM_RUN_TIME = 1.0f;

    m_Animation.addAnimationSequence({ PlayerAnimations::IDLE_1, PlayerAnimations::IDLE_2,
                                       PlayerAnimations::IDLE_3, PlayerAnimations::IDLE_4 },
                                     ANIM_IDLE_TIME);

    m_Animation.addAnimationSequence({ PlayerAnimations::RUN_RIGHT_1, PlayerAnimations::RUN_RIGHT_2,
                                       PlayerAnimations::RUN_RIGHT_3,
                                       PlayerAnimations::RUN_RIGHT_4 },
                                     ANIM_RUN_TIME);

    m_Animation.addAnimationSequence({ PlayerAnimations::RUN_LEFT_1, PlayerAnimations::RUN_LEFT_2,
                                       PlayerAnimations::RUN_LEFT_3, PlayerAnimations::RUN_LEFT_4 },
                                     ANIM_RUN_TIME);

    m_Animation.addConditionalTransition(
        PlayerAnimations::RUNNING, PlayerAnimations::IDLE_1,
        [&acc = m_Acc, &vel = m_Vel](PlayerAnimations current, float timeElapsed) {
            if (acc.x == 0 || vel.x == 0)
                return true;
            else
                return false;
        });

    m_Animation.addConditionalTransition({ PlayerAnimations::IDLE, PlayerAnimations::RUN_LEFT },
                                         PlayerAnimations::RUN_RIGHT_1,
                                         [&acc = m_Acc, &vel = m_Vel, &facingRight = m_FacingRight](
                                             PlayerAnimations current, float timeElapsed) {
                                             if (acc.x > 0 && vel.x > 0)
                                             {
                                                 facingRight = true;
                                                 return true;
                                             }
                                             else
                                                 return false;
                                         });

    m_Animation.addConditionalTransition({ PlayerAnimations::IDLE, PlayerAnimations::RUN_RIGHT },
                                         PlayerAnimations::RUN_LEFT_1,
                                         [&acc = m_Acc, &vel = m_Vel, &facingRight = m_FacingRight](
                                             PlayerAnimations current, float timeElapsed) {
                                             if (acc.x < 0 && vel.x < 0)
                                             {
                                                 facingRight = false;
                                                 return true;
                                             }
                                             else
                                                 return false;
                                         });
}
