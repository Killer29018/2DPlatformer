#include "TileManager.hpp"

#include "imgui.h"

#include "../AABB.hpp"
#include "../Camera.hpp"
#include "../events/Events.hpp"
#include "../tiles/Tile.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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
        for (auto& pair : m_Tiles)
        {
            Tile t = pair.second;
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

            for (auto& pair : m_Tiles)
            {
                pair.second.render(s_Shader);
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

            uint32_t index = 0;
            for (auto& t : m_Tiles)
            {
                tileRoot[index] = t.second.getSaveState();
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

                m_Tiles.insert(std::make_pair(t.getPosition(), t));
            }

            break;
        }
    default:
        break;
    }
}
void TileManager::setTile(glm::vec3 position, glm::ivec2 size, TileType type)
{
    auto end = m_Tiles.end();
    auto it = m_Tiles.begin();
    while (it != end)
    {
        Tile t = it->second;
        // Is there already a block at that position
        bool matchingDepth = t.containsPositionIncludeDepth(position);
        bool matching = t.containsPositionExcludeDepth(position);

        bool replace = (matching && !matchingDepth && t.getType() == type) ||
                       (matchingDepth && t.getType() != type);

        if (replace)
        {
            // Expand the tile and add the new tiles to the end of the array
            // Remove the old tile from the list

            std::vector<Tile> newTiles = expandTile(t);
            m_Tiles.erase(it);

            for (const Tile& t : newTiles)
            {
                glm::vec3 tilePos = t.getPosition();
                if (tilePos.x != position.x || tilePos.y != position.y)
                {
                    m_Tiles.insert(std::make_pair(t.getPosition(), t));
                }
            }
        }
        else
        {
            it++;
        }
    }

    m_Tiles.insert(std::make_pair(position, Tile{ position, size, type }));
}

std::optional<Tile> TileManager::getTile(glm::vec3 position)
{
    std::optional<Tile> t;
    auto pos = m_Tiles.find(position);
    if (pos != m_Tiles.end())
    {
        t = m_Tiles.at(position);
    }
    return t;
}

void TileManager::removeTile(glm::vec3 position)
{
    auto end = m_Tiles.end();
    auto it = m_Tiles.begin();
    while (it != end)
    {
        Tile t = it->second;
        if (t.containsPositionExcludeDepth(position))
        {
            Tile removedTile = t;
            m_Tiles.erase(it);

            std::vector<Tile> newTiles = expandTile(removedTile);

            for (const Tile& t : newTiles)
            {
                glm::vec3 tPos = t.getPosition();
                if (tPos.x != position.x || tPos.y != position.y)
                {
                    m_Tiles.insert(std::make_pair(t.getPosition(), t));
                }
            }
        }
        else
        {
            it++;
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
