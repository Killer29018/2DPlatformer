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
    glm::vec3 m_Gravity = { 0.0, -12, 0.0 };

    Shader m_Shader;
    Tile m_Tile;

    float m_Friction = 0.8f;

    bool m_OnGround = false;
    float m_MovementSpeed = 20.0f;
    float m_JumpSpeed = 800.0f;

    std::unordered_map<int32_t, bool> m_PressedKeys;

    glm::vec3 m_worldRight = { -1.0, 0.0, 0.0 }; // Flipped due to camera
    glm::vec3 m_worldUp = { 0.0, 1.0, 0.0 };

    Camera* m_Camera;
    TileManager* m_Tiles;
};
