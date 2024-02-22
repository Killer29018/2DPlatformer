#pragma once

#include "Tile.hpp"

#include "../events/EventHandler.hpp"
#include "../events/Events.hpp"

#include "../resources/Shader.hpp"
#include "../resources/TextureMap.hpp"

#include <vector>

class TileManager : public EventObserver
{
  public:
    TileManager();
    TileManager(TileManager& other) = delete;
    TileManager(TileManager&& other);

    ~TileManager();

    TileManager& operator=(TileManager&& other);

    void generateMap();

    const TextureMap& getTextureMap() { return m_TextureMap; }

    glm::vec4 checkCollision(glm::vec3 previousPosition, glm::vec2 size, glm::vec2 velocity);

    void setTile(glm::vec3 position, glm::ivec2 size, TileType type);
    void removeTile(glm::vec3 position);

    void receiveEvent(const Event* event) override;

    static glm::vec2 worldPositionToTilePosition(glm::vec2 position);
    static glm::vec2 tilePositionToWorldPosition(glm::vec2 position);

  private:
    static void generateShader();

    std::vector<Tile> expandTile(Tile tile);

  private:
    std::vector<Tile> m_Tiles;

    static Shader s_Shader;
    static bool s_ShaderInitialized;

    TextureMap m_TextureMap;

    const glm::ivec2 m_TileSize{ 32, 32 };
};
