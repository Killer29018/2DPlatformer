#pragma once

#include "Tile.hpp"

#include "../events/EventHandler.hpp"
#include "../events/Events.hpp"

#include "../resources/Shader.hpp"
#include "../resources/TextureMap.hpp"

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

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
    std::optional<Tile> getTile(glm::vec3 position);
    void removeTile(glm::vec3 position);

    void receiveEvent(const Event* event) override;

    static glm::vec2 worldPositionToTilePosition(glm::vec2 position);
    static glm::vec2 tilePositionToWorldPosition(glm::vec2 position);

  private:
    static void generateShader();

    void optimizeTiles();

    std::unordered_map<glm::vec3, Tile>
    mergeTilesDirection(std::unordered_map<glm::vec3, Tile> tiles, bool mergeRight);

    Tile mergeTiles(Tile t1, Tile t2, bool mergeRight);

    std::vector<Tile> expandTile(Tile tile);

  private:
    std::unordered_map<glm::vec3, Tile> m_Tiles;

    bool m_ShowTileBlock = false;

    static Shader s_Shader;
    static bool s_ShaderInitialized;

    TextureMap m_TextureMap;

    const glm::ivec2 m_TileSize{ 32, 32 };
};
