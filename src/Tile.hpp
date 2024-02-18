#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "TileTypes.hpp"

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
    void setWorldPosition(glm::vec3 position);

    glm::vec3 getPosition() { return m_Position; }
    glm::ivec2 getSize() { return m_BlockSize; }
    TileType getType() { return m_Type; }

    glm::vec3 getWorldPosition();
    glm::vec2 getWorldSize();

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
