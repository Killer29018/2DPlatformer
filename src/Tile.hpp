#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"

class Tile
{
  public:
    static float s_TileSize;

    int collided = 0;

    Tile();
    Tile(glm::vec2 position, float depth = 0.0);
    Tile(glm::vec2 position, glm::ivec2 blockSize, float depth = 0.0);

    void render(Shader& shader);

    void setPosition(glm::vec3 position) { m_Position = position; }
    void setWorldPosition(glm::vec3 position);

    glm::vec3 getWorldPosition();
    glm::vec2 getWorldSize();

    static void generateMesh();

  private:
    glm::vec3 m_Position;
    glm::ivec2 m_BlockSize;

    static uint32_t s_VertexCount;
    static uint32_t s_VAO;
    static uint32_t s_VBO;
    static bool s_TileInitialized;
};
