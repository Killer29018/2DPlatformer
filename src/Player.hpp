#pragma once

#include "Animation.hpp"
#include "Camera.hpp"
#include "events/EventHandler.hpp"
#include "resources/Texture2D.hpp"
#include "tiles/Tile.hpp"
#include "tiles/TileManager.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

enum class PlayerAnimations { PLAYER_IDLE };
const std::unordered_map<PlayerAnimations, glm::ivec2> PlayerAnimationToVec = {
    {PlayerAnimations::PLAYER_IDLE, { 1, 0 }}
};

class Player : public EventObserver
{
  public:
    Player();
    Player(Camera* camera, TileManager* tiles);
    Player(Player& other) = delete;
    Player(Player&& other);

    ~Player();

    Player& operator=(Player& other) = delete;
    Player& operator=(Player&& other);

    glm::vec3 getPosition() { return m_Position; }

    void receiveEvent(const Event* event) override;

  private:
    glm::vec3 m_Position;
    glm::vec3 m_Vel;
    glm::vec3 m_Acc;
    glm::vec3 m_Gravity = { 0.0, -14, 0.0 };

    Shader m_Shader;
    Animation<PlayerAnimations> m_Animation;

    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_VertexCount;

    glm::vec2 m_Size{ Tile::s_TileSize * 0.9, Tile::s_TileSize * 1.9 };

    float m_Damping = 6.0f;

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
