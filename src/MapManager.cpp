#include "MapManager.hpp"

#include "Events.hpp"
#include "imgui.h"

MapManager::MapManager() {}

MapManager::MapManager(Player* player, TileManager* tileManager, Window* window)
    : m_Player(player), m_TileManager(tileManager), m_Window(window)
{
    m_GhostShader.compileFromPath("res/shaders/ghostTile.vert.glsl",
                                  "res/shaders/ghostTile.frag.glsl");

    m_GhostTile = GhostTile(glm::vec3{ 0.0f }, glm::ivec2{ 1 }, TileType::STONE_CENTER);
}

void MapManager::receiveEvent(const Event* event)
{
    static glm::vec2 mousePos;
    static glm::vec2 mousePercent;
    static float blockDepth;

    switch (event->getType())
    {
    case EventType::Update:
        {
            mousePos = mousePercent;

            glm::ivec2 windowSize = m_Window->getSize();
            float aspectRatio = (float)windowSize.x / (float)windowSize.y;
            float halfHeight = 1.f;
            float halfWidth = aspectRatio * halfHeight;

            glm::vec2 halfSize = { halfWidth, -halfHeight };

            mousePos *= (halfSize * 2.f);
            mousePos -= halfSize;

            glm::vec3 playerPosition = m_Player->getPosition();
            mousePos += glm::vec2{ playerPosition.x, playerPosition.y };
            mousePos = TileManager::worldPositionToTilePosition(mousePos);

            m_GhostTile.setPosition(glm::vec3{ mousePos.x, mousePos.y, 10.0f });
            break;
        }
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            m_GhostShader.bind();

            m_TileManager->getTextureMap().bind();

            m_GhostShader.setMat4("u_View", renderEvent->camera->getViewMatrix());
            m_GhostShader.setMat4("u_Projection", renderEvent->camera->getProjectionMatrix());
            m_GhostShader.setIVec2("u_TilemapSize",
                                   m_TileManager->getTextureMap().getTileDimensions());

            m_GhostTile.render(m_GhostShader);
            break;
        }
    case EventType::MouseMove:
        {
            const MouseMoveEvent* mmEvent = reinterpret_cast<const MouseMoveEvent*>(event);

            mousePercent = glm::vec2{ mmEvent->xPercent, mmEvent->yPercent };

            break;
        }
    case EventType::MousePress:
        {
            const MousePressEvent* mpEvent = reinterpret_cast<const MousePressEvent*>(event);

            if (mpEvent->leftClick)
            {
                // Set Block
                glm::vec3 ghostPosition = m_GhostTile.getPosition();
                glm::vec3 position = glm::vec3{ ghostPosition.x, ghostPosition.y, blockDepth };
                m_TileManager->setTile(position, m_GhostTile.getSize(), m_GhostTile.getType());
            }
            if (mpEvent->rightClick)
            {
                glm::vec3 ghostPosition = m_GhostTile.getPosition();
                glm::vec3 position = glm::vec3{ ghostPosition.x, ghostPosition.y, blockDepth };
                m_TileManager->removeTile(position);
            }
            break;
        }
    case EventType::ImGuiRender:
        {
            if (ImGui::Begin("MapManager"))
            {
                ImGui::Text("Mouse Pos: (%f, %f)", mousePos.x, mousePos.y);
                ImGui::Text("Mouse Percent: (%f, %f)", mousePercent.x, mousePercent.y);
                ImGui::End();
            }
            break;
        }
    default:
        break;
    }
}
