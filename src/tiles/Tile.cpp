#include "Tile.hpp"

#include <glad/gl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

uint32_t Tile::s_VertexCount = 0;
uint32_t Tile::s_VAO;
uint32_t Tile::s_VBO;
bool Tile::s_TileInitialized = false;

Tile::Tile() : m_Position(0.0, 0.0, 0.0), m_Size(1, 1), m_Type(TileType::NONE) {}

Tile::Tile(glm::vec3 position, TileType map) : m_Position(position), m_Size(1, 1), m_Type(map) {}

Tile::Tile(glm::vec3 position, glm::ivec2 size, TileType map)
    : m_Position(position), m_Size(size), m_Type(map)
{
}

void Tile::render(Shader& shader)
{
    glm::mat4 model{ 1.0f };
    model = glm::scale(model, glm::vec3(m_Size.x * s_TileSize, m_Size.y * s_TileSize, 1.0));
    model = glm::translate(
        model, glm::vec3(m_Position.x / m_Size.x, m_Position.y / m_Size.y, m_Position.z));

    shader.setMat4("u_Model", model);
    const std::vector<glm::ivec2>& coords = TileTypeToVec.at(m_Type);

    shader.setIVec2Array("u_TileIndex", coords);
    shader.setInt("u_TileIndexSize", coords.size());
    shader.setIVec2("u_TileSize", m_Size);

    shader.setInt("u_BlockID", (int)std::hash<glm::vec3>{}(m_Position));

    glBindVertexArray(s_VAO);

    glDrawArrays(GL_TRIANGLES, 0, s_VertexCount);
}

void Tile::setWorldPosition(glm::vec3 position)
{
    m_Position = glm::vec3(position.x / s_TileSize, position.y / s_TileSize, position.z);
}

Json::Value Tile::getSaveState()
{
    Json::Value root;
    root["position"]["x"] = m_Position.x;
    root["position"]["y"] = m_Position.y;
    root["position"]["z"] = m_Position.z;

    root["size"]["x"] = m_Size.x;
    root["size"]["y"] = m_Size.y;

    root["tileType"] = static_cast<int>(m_Type);

    return root;
}

void Tile::loadSaveState(Json::Value root)
{
    m_Position.x = root["position"]["x"].asFloat();
    m_Position.y = root["position"]["y"].asFloat();
    m_Position.z = root["position"]["z"].asFloat();

    m_Size.x = root["size"]["x"].asInt();
    m_Size.y = root["size"]["y"].asInt();

    m_Type = static_cast<TileType>(root["tileType"].asInt());
}

bool Tile::containsPositionIncludeDepth(glm::vec3 position) const
{
    return getPosition().z == position.z && containsPositionExcludeDepth(position);
}

bool Tile::containsPositionExcludeDepth(glm::vec3 position) const
{
    glm::vec3 pos = getPosition();
    glm::vec2 size = getSize();

    return (pos.x <= position.x && pos.x + size.x > position.x && //
            pos.y <= position.y && pos.y + size.y > position.y);
}

glm::vec3 Tile::getWorldPosition() const
{
    return glm::vec3(m_Position.x * s_TileSize, m_Position.y * s_TileSize, m_Position.z);
}

glm::vec2 Tile::getWorldSize() const
{
    return glm::vec2(m_Size.x * s_TileSize, m_Size.y * s_TileSize);
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

    assert(TileTypeToVec.size() == static_cast<int>(TileType::LAST) &&
           "Missing TileTypeToVec entries or TileType entries");

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
