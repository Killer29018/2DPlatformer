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
  // {   { 1.0f, 1.0f },  { 2, 1 }}, //
        {   { 0.0f, 2.0f },  { 2, 2 }}, //
        {  { -1.0f, 4.0f },  { 3, 3 }}, //
        {  { -2.0f, 7.0f },  { 5, 5 }}, //
        {  { 0.0f, -3.0f }, { 10, 3 }},
        {{ -10.0f, -3.0f }, { 10, 3 }},
        {   { 5.0f, 3.0f },  { 2, 1 }},
        {   { 7.0f, 6.0f },  { 2, 1 }},
        {   { 7.0f, 6.0f },  { 2, 1 }},
    };
}

glm::vec4 TileManager::checkCollision(glm::vec2 previousPosition, glm::vec2 size,
                                      glm::vec2 velocity)
{
    glm::vec4 afterBoundingBox = { previousPosition.x + velocity.x, previousPosition.y + velocity.y,
                                   size.x, size.y };
    const int iterations = 2;
    for (int i = 0; i < iterations; i++)
    {
        for (Tile& t : m_Tiles)
        {
            t.collided = 0;
        }

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
                t.collided = 1;

                glm::vec2 offsets =
                    AABB::calculateOffsets(afterBoundingBox, tileBoundingBox, velocity);

                xOffset = offsets.x;
                yOffset = offsets.y;

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
    srand(time(0));
    switch (event->getType())
    {
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            s_Shader.bind();

            s_Shader.setMat4("u_View", renderEvent->camera->getViewMatrix());
            s_Shader.setMat4("u_Projection", renderEvent->camera->getProjectionMatrix());
            s_Shader.setInt("u_TotalRows", 2);
            s_Shader.setInt("u_TotalCols", 2);

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
