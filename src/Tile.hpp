#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "TileTypes.hpp"
#include "json/json.h"

class Tile
{
  public:
    constexpr static float s_TileSize = 0.1f;

    Tile();
    Tile(glm::vec3 position, TileType map);
    Tile(glm::vec3 position, glm::ivec2 blockSize, TileType map);

    void render(Shader& shader);

    void setTile(TileType type) { m_Type = type; }
    void setPosition(glm::vec3 position) { m_Position = position; }
    void setDepth(float depth) { m_Position.z = depth; }
    void setWorldPosition(glm::vec3 position);

    Json::Value getSaveState();

    glm::vec3 getPosition() const { return m_Position; }
    glm::ivec2 getSize() const { return m_BlockSize; }
    TileType getType() const { return m_Type; }

    bool containsPositionIncludeDepth(glm::vec3 position) const;
    bool containsPositionExcludeDepth(glm::vec3 position) const;

    glm::vec3 getWorldPosition() const;
    glm::vec2 getWorldSize() const;

    static void generateMesh();

  protected:
    glm::vec3 m_Position;
    glm::ivec2 m_BlockSize;

    TileType m_Type;

    static uint32_t s_VertexCount;
    static uint32_t s_VAO;
    static uint32_t s_VBO;
    static bool s_TileInitialized;
};
