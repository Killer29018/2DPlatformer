#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Tile.hpp"
#include "TileManager.hpp"
#include <glm/glm.hpp>

class Player : public EventObserver
{
  public:
    Player();
    Player(Camera* camera, TileManager* tiles);

    void receiveEvent(const Event* event) override;

  private:
    glm::vec3 m_Position;
    glm::vec3 m_Vel;
    glm::vec3 m_Acc;
    glm::vec3 m_Gravity = { 0.0, -14, 0.0 };

    Shader m_Shader;
    Texture2D m_Texture;

    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_VertexCount;

    glm::vec2 m_Size{ Tile::s_TileSize * 0.9, Tile::s_TileSize * 1.9 };

    float m_Damping = 0.9f;

    bool m_OnGround = true;
    float m_MovementSpeed = 20.0f;
    float m_JumpSpeed = 10.0f;

    std::unordered_map<int32_t, bool> m_PressedKeys;

    glm::vec3 m_worldRight = { 1.0, 0.0, 0.0 };
    glm::vec3 m_worldUp = { 0.0, 1.0, 0.0 };

    Camera* m_Camera;
    TileManager* m_Tiles;

  private:
    void setupPlayerData();
};
