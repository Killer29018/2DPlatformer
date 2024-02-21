#include "TileManager.hpp"

#include "Camera.hpp"

#include "AABB.hpp"
#include "Events.hpp"
#include "Tile.hpp"
#include "imgui.h"

#include <format>
#include <iostream>

Shader TileManager::s_Shader;
bool TileManager::s_ShaderInitialized = false;

TileManager::TileManager() {}

TileManager::TileManager(TileManager&& other)
{
    m_Tiles = other.m_Tiles;
    m_TextureMap = std::move(other.m_TextureMap);

    other.m_Tiles.clear();
}

TileManager::~TileManager() {}

TileManager& TileManager::operator=(TileManager&& other)
{

    m_Tiles = other.m_Tiles;
    m_TextureMap = std::move(other.m_TextureMap);

    other.m_Tiles.clear();

    return *this;
}

void TileManager::generateMap()
{
    generateShader();

    Tile::generateMesh();

    m_Tiles = {
        {{ -5.0f, 0.0f, 0.0f },    { 1, 1 }, TileType::STONE_TOP_LEFT        },
        { { -4.0f, 0.0f, 0.0f },   { 9, 1 }, TileType::STONE_TOP_CENTER      },
        { { 5.0f, 0.0f, 0.0f },    { 1, 1 }, TileType::STONE_TOP_RIGHT       },

        { { -5.0f, -3.0f, 0.0f },  { 1, 3 }, TileType::STONE_CENTER_LEFT     },
        { { -4.0f, -3.0f, 0.0f },  { 9, 3 }, TileType::STONE_CENTER          },
        { { 5.0f, -3.0f, 0.0f },   { 1, 3 }, TileType::STONE_CENTER_RIGHT    },

        { { -5.0f, -4.0f, 0.0f },  { 1, 1 }, TileType::STONE_BOTTOM_LEFT     },
        { { -4.0f, -4.0f, 0.0f },  { 9, 1 }, TileType::STONE_BOTTOM_CENTER   },
        { { 5.0f, -4.0f, 0.0f },   { 1, 1 }, TileType::STONE_BOTTOM_RIGHT    },

        { { 10.0f, 3.0f, 1.0f },   { 1, 1 }, TileType::ABOVE_GRASS_TOP_LEFT  },
        { { 11.0f, 3.0f, 1.0f },   { 9, 1 }, TileType::ABOVE_GRASS_TOP_CENTER},
        { { 20.0f, 3.0f, 1.0f },   { 1, 1 }, TileType::ABOVE_GRASS_TOP_RIGHT },

        { { 10.0f, 2.0f, 0.0f },   { 1, 1 }, TileType::STONE_GRASS_TOP_LEFT  },
        { { 11.0f, 2.0f, 0.0f },   { 9, 1 }, TileType::STONE_GRASS_TOP_CENTER},
        { { 20.0f, 2.0f, 0.0f },   { 1, 1 }, TileType::STONE_GRASS_TOP_RIGHT },

        { { 10.0f, -1.0f, 1.0f },  { 1, 3 }, TileType::STONE_CENTER_LEFT     },
        { { 11.0f, -1.0f, -1.0f }, { 9, 3 }, TileType::STONE_CENTER          },
        { { 20.0f, -1.0f, 0.0f },  { 1, 3 }, TileType::STONE_CENTER_RIGHT    },

        { { 10.0f, -2.0f, 0.0f },  { 1, 1 }, TileType::STONE_BOTTOM_LEFT     },
        { { 11.0f, -2.0f, 0.0f },  { 9, 1 }, TileType::STONE_BOTTOM_CENTER   },
        { { 20.0f, -2.0f, 0.0f },  { 1, 1 }, TileType::STONE_BOTTOM_RIGHT    },
    };

    m_TextureMap.compileFromPath("res/textures/Tilemap.png", 32, 32);
}

glm::vec4 TileManager::checkCollision(glm::vec3 previousPosition, glm::vec2 size,
                                      glm::vec2 velocity)
{
    glm::vec4 afterBoundingBox = { previousPosition.x + velocity.x, previousPosition.y + velocity.y,
                                   size.x, size.y };
    const int iterations = 2;
    for (int i = 0; i < iterations; i++)
    {
        for (Tile& t : m_Tiles)
        {
            glm::vec3 position = t.getWorldPosition();

            if (previousPosition.z != position.z) continue;

            glm::vec2 size = t.getWorldSize();

            glm::vec4 tileBoundingBox = { position.x, position.y, size.x, size.y };

            if (AABB::collision(afterBoundingBox, tileBoundingBox))
            {
                glm::vec2 offsets =
                    AABB::calculateOffsets(afterBoundingBox, tileBoundingBox, velocity);

                if (fabs(offsets.y) < fabs(offsets.x))
                {
                    afterBoundingBox.y += offsets.y;
                    velocity.y = 0;
                }
                else
                {
                    afterBoundingBox.x += offsets.x;
                    velocity.x = 0;
                }
            }
        }
    }

    return glm::vec4{ afterBoundingBox.x, afterBoundingBox.y, velocity.x, velocity.y };
}

void TileManager::receiveEvent(const Event* event)
{
    static glm::vec2 mousePos;

    switch (event->getType())
    {
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            s_Shader.bind();

            m_TextureMap.bind();

            s_Shader.setMat4("u_View", renderEvent->camera->getViewMatrix());
            s_Shader.setMat4("u_Projection", renderEvent->camera->getProjectionMatrix());
            s_Shader.setIVec2("u_TilemapSize", m_TextureMap.getTileDimensions());

            for (Tile& t : m_Tiles)
            {
                t.render(s_Shader);
            }

            break;
        }
    case EventType::ImGuiRender:
        {
            if (ImGui::Begin("Tile Manager"))
            {
                ImGui::Text("Tile Count: %ld", m_Tiles.size());
                ImGui::End();
            }
            break;
        }
    case EventType::SaveGame:
        {
            const SaveGameEvent* sgEvent = reinterpret_cast<const SaveGameEvent*>(event);
            Json::Value tileRoot;

            for (size_t i = 0; i < m_Tiles.size(); i++)
            {
                tileRoot[(uint32_t)i] = m_Tiles[i].getSaveState();
            }

            (*sgEvent->root)["Tiles"] = tileRoot;

            break;
        }
    case EventType::LoadGame:
        {
            const LoadGameEvent* lgEvent = reinterpret_cast<const LoadGameEvent*>(event);

            Json::Value tiles = (*lgEvent->root)["Tiles"];

            m_Tiles.clear();
            for (Json::Value tile : tiles)
            {
                Tile t;
                t.loadSaveState(tile);

                m_Tiles.push_back(t);
            }

            break;
        }
    default:
        break;
    }
}
void TileManager::setTile(glm::vec3 position, glm::ivec2 size, TileType type)
{
    auto end = m_Tiles.rend();
    for (auto it = m_Tiles.rbegin(); it != end; it++)
    {
        // Is there already a block at that position
        bool matchingDepth = it->containsPositionIncludeDepth(position);
        bool matching = it->containsPositionExcludeDepth(position);

        bool replace = (matching && !matchingDepth && it->getType() == type) ||
                       (matchingDepth && it->getType() != type);

        if (replace)
        {
            // Expand the tile and add the new tiles to the end of the array
            // Remove the old tile from the list

            std::vector<Tile> newTiles = expandTile(*it);
            m_Tiles.erase(std::next(it).base());

            for (const Tile& t : newTiles)
            {
                glm::vec3 tilePos = t.getPosition();
                if (tilePos.x != position.x || tilePos.y != position.y)
                {
                    m_Tiles.push_back(t);
                }
            }
        }
    }
    m_Tiles.emplace_back(position, size, type);
}

void TileManager::removeTile(glm::vec3 position)
{
    auto end = m_Tiles.rend();
    for (auto it = m_Tiles.rbegin(); it != end; it++)
    {
        if (it->containsPositionExcludeDepth(position))
        {
            Tile removedTile = *it;
            m_Tiles.erase(std::next(it).base());

            std::vector<Tile> newTiles = expandTile(removedTile);

            for (const Tile& t : newTiles)
            {
                glm::vec3 tPos = t.getPosition();
                if (tPos.x != position.x || tPos.y != position.y)
                {
                    m_Tiles.push_back(t);
                }
            }
        }
    }
}

glm::vec2 TileManager::worldPositionToTilePosition(glm::vec2 position)
{
    position /= Tile::s_TileSize;
    position.x = floor(position.x);
    position.y = floor(position.y);

    return position;
}

glm::vec2 TileManager::tilePositionToWorldPosition(glm::vec2 position) { return position; }

void TileManager::generateShader()
{
    if (!s_ShaderInitialized)
        s_ShaderInitialized = true;
    else
        return;

    s_ShaderInitialized = true;
    s_Shader.compileFromPath("res/shaders/tile.vert.glsl", "res/shaders/tile.frag.glsl");
}
std::vector<Tile> TileManager::expandTile(Tile tile)
{
    glm::vec3 position = tile.getPosition();
    glm::ivec2 size = tile.getSize();
    std::vector<Tile> newTiles;

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            glm::vec3 tilePosition = { position.x + x, position.y + y, position.z };
            newTiles.emplace_back(tilePosition, glm::ivec2{ 1, 1 }, tile.getType());
        }
    }

    return newTiles;
}
