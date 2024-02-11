#pragma once

#include "EventHandler.hpp"
#include "Events.hpp"
#include "Shader.hpp"
#include "Tile.hpp"
#include <vector>

class TileManager : public EventObserver
{
  public:
    TileManager();

    void generateMap();

    glm::vec4 checkCollision(glm::vec2 previousPosition, glm::vec2 size, glm::vec2 velocity);

    void receiveEvent(const Event* event) override;

  private:
    std::vector<Tile> m_Tiles;

    static Shader s_Shader;
    static bool s_Initialized;

  private:
    static void generateShader();
};
