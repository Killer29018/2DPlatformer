#include "Tile.hpp"

#include <glad/gl.h>

float Tile::s_TileSize = 0.1f;
uint32_t Tile::s_VertexCount = 0;
uint32_t Tile::s_VAO;
uint32_t Tile::s_VBO;
bool Tile::s_TileInitialized = false;

Tile::Tile() : m_Position(0.0, 0.0, 0.0), m_BlockSize(1, 1), m_TileType(TileMap::NONE) {}

Tile::Tile(glm::vec2 position, TileMap map, float depth)
    : m_Position(position.x, position.y, depth), m_BlockSize(1, 1), m_TileType(map)
{
    generateMesh();
}

Tile::Tile(glm::vec2 position, glm::ivec2 blockSize, TileMap map, float depth)
    : m_Position(position.x, position.y, depth), m_BlockSize(blockSize), m_TileType(map)
{
    generateMesh();
}

void Tile::render(Shader& shader)
{
    glm::mat4 model{ 1.0f };
    model =
        glm::scale(model, glm::vec3(m_BlockSize.x * s_TileSize, m_BlockSize.y * s_TileSize, 1.0));
    model = glm::translate(
        model, glm::vec3(m_Position.x / m_BlockSize.x, m_Position.y / m_BlockSize.y, m_Position.z));

    shader.setMat4("u_Model", model);
    shader.setIVec2("u_TileIndex", TileMapToVec.at(m_TileType));

    shader.setIVec2("u_TileSize", m_BlockSize);

    glBindVertexArray(s_VAO);

    glDrawArrays(GL_TRIANGLES, 0, s_VertexCount);
}

void Tile::setWorldPosition(glm::vec3 position)
{
    m_Position = glm::vec3(position.x / s_TileSize, position.y / s_TileSize, position.z);
}

glm::vec3 Tile::getWorldPosition()
{
    return glm::vec3(m_Position.x * s_TileSize, m_Position.y * s_TileSize, m_Position.z);
}

glm::vec2 Tile::getWorldSize()
{
    return glm::vec2(m_BlockSize.x * s_TileSize, m_BlockSize.y * s_TileSize);
}

void Tile::generateMesh()
{
    if (!s_TileInitialized)
    {
        s_TileInitialized = true;
    }
    else
    {
        return;
    }

    assert(TileMapToVec.size() == static_cast<int>(TileMap::LAST) &&
           "Missing TileMapToVec entries or TileMap entries");

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);

    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);

    std::vector<float> vertices = {
        0.0, 0.0, 0.0f, 1.0f, //
        0.0, 1.0, 0.0f, 0.0f, //
        1.0, 1.0, 1.0f, 0.0f, //
        0.0, 0.0, 0.0f, 1.0f, //
        1.0, 1.0, 1.0f, 0.0f, //
        1.0, 0.0, 1.0f, 1.0f, //
    };

    s_VertexCount = 6;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}
