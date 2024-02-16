#include "TileManager.hpp"

#include "Camera.hpp"

#include "AABB.hpp"
#include "glm/fwd.hpp"
#include <iterator>

Shader TileManager::s_Shader;
bool TileManager::s_Initialized = false;

TileManager::TileManager() {}

void TileManager::generateMap()
{
    generateShader();

    m_Tiles = {
        { { -5.0f, 0.0f }, { 1, 1 },      TileMap::STONE_TOP_LEFT},
        { { -4.0f, 0.0f }, { 9, 1 },    TileMap::STONE_TOP_CENTER},
        {  { 5.0f, 0.0f }, { 1, 1 },     TileMap::STONE_TOP_RIGHT},
 //
        {{ -5.0f, -3.0f }, { 1, 3 },   TileMap::STONE_CENTER_LEFT},
        {{ -4.0f, -3.0f }, { 9, 3 },        TileMap::STONE_CENTER},
        { { 5.0f, -3.0f }, { 1, 3 },  TileMap::STONE_CENTER_RIGHT},

        {{ -5.0f, -4.0f }, { 1, 1 },   TileMap::STONE_BOTTOM_LEFT},
        {{ -4.0f, -4.0f }, { 9, 1 }, TileMap::STONE_BOTTOM_CENTER},
        { { 5.0f, -4.0f }, { 1, 1 },  TileMap::STONE_BOTTOM_RIGHT},
    };

    m_TextureMap.compileFromPath("res/textures/Tilemap.png", 4, 4);
}

glm::vec4 TileManager::checkCollision(glm::vec2 previousPosition, glm::vec2 size,
                                      glm::vec2 velocity)
{
    glm::vec4 afterBoundingBox = { previousPosition.x + velocity.x, previousPosition.y + velocity.y,
                                   size.x, size.y };
    const int iterations = 2;
    for (int i = 0; i < iterations; i++)
    {
        float xOffset = 0;
        float yOffset = 0;
        bool collision = false;

        for (Tile& t : m_Tiles)
        {
            glm::vec3 position = t.getWorldPosition();
            glm::vec2 size = t.getWorldSize();

            glm::vec4 tileBoundingBox = { position.x, position.y, size.x, size.y };

            if (AABB::collision(afterBoundingBox, tileBoundingBox))
            {
                glm::vec2 offsets =
                    AABB::calculateOffsets(afterBoundingBox, tileBoundingBox, velocity);

                xOffset += offsets.x;
                yOffset += offsets.y;

                collision = true;
            }
        }

        if (collision)
        {
            if (fabs(yOffset) < fabs(xOffset))
            {
                velocity.y = 0;
                xOffset = 0;
            }
            else
            {
                velocity.x = 0;
                yOffset = 0;
            }
        }
        else
        {
            break;
        }

        afterBoundingBox.x += xOffset;
        afterBoundingBox.y += yOffset;
    }

    return glm::vec4{ afterBoundingBox.x, afterBoundingBox.y, velocity.x, velocity.y };
}

void TileManager::receiveEvent(const Event* event)
{
    srand(0);
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
    default:
        break;
    }
}

void TileManager::generateShader()
{
    if (!s_Initialized)
        s_Initialized = true;
    else
        return;

    s_Initialized = true;
    s_Shader.compileFromPath("res/shaders/tile.vert.glsl", "res/shaders/tile.frag.glsl");
}
