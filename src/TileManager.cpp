#include "TileManager.hpp"

#include "Camera.hpp"

Shader TileManager::s_Shader;
bool TileManager::s_Initialized = false;

TileManager::TileManager() {}

void TileManager::generateMap()
{
    generateShader();

    m_Tiles = {
        {  { 0.0f, 0.0f }, { 1, 1 }}, //
        {  { 1.0f, 1.0f }, { 2, 1 }}, //
        {  { 0.0f, 0.0f }, { 2, 1 }}, //
        {  { 0.0f, 2.0f }, { 2, 2 }}, //
        { { -1.0f, 4.0f }, { 3, 3 }}, //
        { { -2.0f, 7.0f }, { 5, 5 }}, //
        {{ -2.0f, 12.0f }, { 7, 7 }}, //
        {  { 3.0f, 7.0f }, { 3, 4 }}, //
    };
}

glm::vec2 TileManager::checkCollision(glm::vec2 position, glm::vec2 size)
{
    glm::vec4 boundingBox = { position.x, position.y, position.x + size.x, position.y + size.y };

    for (Tile& t : m_Tiles)
    {
        t.collided = 0;
    }

    for (Tile& t : m_Tiles)
    {
        glm::vec3 position = t.getWorldPosition();
        glm::vec2 size = t.getWorldSize();

        float EPS = 0.0001;

        glm::vec4 tileBoundingBox = { position.x, position.y, position.x + size.x,
                                      position.y + size.y };

        if (boundingBox.x < tileBoundingBox.z && boundingBox.z > tileBoundingBox.x &&
            boundingBox.y < tileBoundingBox.w && boundingBox.w > tileBoundingBox.y)
        {
            t.collided = 1;

            float xOffset = 0;
            float yOffset = 0;

            float xLeft = tileBoundingBox.x - boundingBox.z;
            float xRight = tileBoundingBox.z - boundingBox.x;

            float yUp = tileBoundingBox.w - boundingBox.y;
            float yDown = tileBoundingBox.y - boundingBox.w;

            if (fabs(xLeft) < fabs(xRight))
                xOffset = xLeft - EPS;
            else
                xOffset = xRight + EPS;

            if (fabs(yUp) < fabs(yDown))
                yOffset = yUp + EPS;
            else
                yOffset = yDown - EPS;

            if (yOffset < xOffset && fabs(yOffset) > EPS)
                xOffset = 0;
            else
                yOffset = 0;

            static uint32_t count = 0;
            printf("Collision: %d\n", count++);

            return { xOffset, yOffset };
        }
    }
    return { 0.0f, 0.0f };
}

void TileManager::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            s_Shader.setMat4("u_View", renderEvent->camera->getViewMatrix());
            s_Shader.setMat4("u_Projection", renderEvent->camera->getProjectionMatrix());

            s_Shader.bind();
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
