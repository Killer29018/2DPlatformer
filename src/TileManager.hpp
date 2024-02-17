#pragma once

#include "EventHandler.hpp"
#include "Events.hpp"
#include "Shader.hpp"
#include "TextureMap.hpp"
#include "Tile.hpp"
#include <vector>

class TileManager : public EventObserver
{
  public:
    TileManager();

    void generateMap();

    glm::vec4 checkCollision(glm::vec3 previousPosition, glm::vec2 size, glm::vec2 velocity);

    void receiveEvent(const Event* event) override;

  private:
    std::vector<Tile> m_Tiles;

    static Shader s_Shader;
    static bool s_Initialized;

    TextureMap m_TextureMap;

  private:
    static void generateShader();

    const glm::ivec2 m_TileSize{ 32, 32 };
};
