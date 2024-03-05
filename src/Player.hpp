#pragma once

#include "Animation.hpp"
#include "Camera.hpp"
#include "events/EventHandler.hpp"
#include "resources/Texture2D.hpp"
#include "tiles/Tile.hpp"
#include "tiles/TileManager.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

enum class PlayerAnimations {
    INVALID,
    IDLE,
    IDLE2,
    RUN_RIGHT_1,
    RUN_RIGHT_2,
    RUN_LEFT_1,
    RUN_LEFT_2
};
const std::unordered_map<PlayerAnimations, glm::ivec2> PlayerAnimationToVec = {
    {PlayerAnimations::INVALID,      { -1, -1 }},
    { PlayerAnimations::IDLE,        { 0, 0 }  },
    { PlayerAnimations::IDLE2,       { 1, 0 }  },
    { PlayerAnimations::RUN_RIGHT_1, { 2, 0 }  },
    { PlayerAnimations::RUN_RIGHT_2, { 3, 0 }  },
    { PlayerAnimations::RUN_LEFT_1,  { 0, 1 }  },
    { PlayerAnimations::RUN_LEFT_2,  { 1, 1 }  },
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

    void init();
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

    void addAnimations();
};
