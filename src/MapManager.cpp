#include "MapManager.hpp"

#include "events/Events.hpp"
#include "imgui.h"
#include "tiles/TileTypes.hpp"

#include <format>
#include <iostream>

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

    static glm::vec3 previousPosition;

    switch (event->getType())
    {
    case EventType::Update:
        {
            const UpdateEvent* uEvent = reinterpret_cast<const UpdateEvent*>(event);
            m_TimeElapsed += uEvent->dt;

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

            m_GhostTile.setPosition(
                glm::vec3{ mousePos.x, mousePos.y, m_GhostTile.getPosition().z });
            break;
        }
    case EventType::Render:
        {
            RenderEvent* renderEvent = (RenderEvent*)event;

            m_GhostShader.bind();

            m_TileManager->getTextureMap().bind();

            m_GhostShader.setFloat("u_TimeElapsed", m_TimeElapsed);
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
    case EventType::MouseDragged:
        {
            const MouseDraggedEvent* mdEvent = reinterpret_cast<const MouseDraggedEvent*>(event);

            if (previousPosition != m_GhostTile.getPosition())
            {
                if (mdEvent->leftClick) placeBlock();
                if (mdEvent->rightClick) removeBlock();

                previousPosition = m_GhostTile.getPosition();
            }

            break;
        }
    case EventType::MousePress:
        {
            const MousePressEvent* mpEvent = reinterpret_cast<const MousePressEvent*>(event);

            if (mpEvent->leftClick)
            {
                placeBlock();
            }
            if (mpEvent->rightClick)
            {
                removeBlock();
            }
            previousPosition = m_GhostTile.getPosition();
            break;
        }
    case EventType::ImGuiRender:
        {
            if (ImGui::Begin("MapManager"))
            {
                ImGui::Text("Mouse Tile Pos: (%f, %f)", mousePos.x, mousePos.y);

                const TextureMap& map = m_TileManager->getTextureMap();
                uint32_t id = map.getEntireTexture().getID();
                glm::ivec2 size = map.getTotalSize();
                glm::ivec2 tileSize = map.getTileSize();
                glm::ivec2 tiles = map.getTileDimensions();

                ImGui::Text("Texture ID: %d", id);

                int depth = (int)m_GhostTile.getPosition().z;
                ImGui::SliderInt("Depth", &depth, -10, 10);
                m_GhostTile.setDepth((float)depth);

                ImVec2 initialPos = ImGui::GetCursorPos();
                ImGui::Image((void*)(intptr_t)id, ImVec2(size.x, size.y));
                ImGui::SetCursorPos(initialPos);

                int32_t xCoord = -1;
                int32_t yCoord = -1;
                bool pressed = false;

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

                for (int y = 0; y < tiles.y; y++)
                {
                    for (int x = 0; x < tiles.x; x++)
                    {
                        ImGui::PushID(y * size.x + x);

                        if (ImGui::Button("##TilemapSelection", ImVec2(tileSize.x, tileSize.y)))
                        {
                            xCoord = x;
                            yCoord = y;
                            pressed = true;
                        }

                        ImGui::PopID();

                        if (x != tiles.x - 1)
                        {
                            ImGui::SameLine();
                        }
                    }
                }

                ImGui::PopStyleVar(1);

                if (pressed)
                {
                    // Find TileType for coordinate
                    glm::ivec2 tileCoord{ xCoord, yCoord };
                    auto result = std::find_if(TileTypeToVec.begin(), TileTypeToVec.end(),
                                               [&](const std::pair<TileType, glm::ivec2>& pair) {
                                                   return pair.second == tileCoord;
                                               });
                    TileType type = TileType::NONE;

                    if (result != TileTypeToVec.end())
                    {
                        type = result->first;
                        m_GhostTile.setType(type);
                    }
                }

                ImGui::End();
            }
            break;
        }
    default:
        break;
    }
}
void MapManager::placeBlock()
{
    glm::vec3 ghostPosition = m_GhostTile.getPosition();
    glm::vec3 position = glm::vec3{ ghostPosition.x, ghostPosition.y, ghostPosition.z };
    m_TileManager->setTile(position, m_GhostTile.getSize(), m_GhostTile.getType());
}

void MapManager::removeBlock()
{
    glm::vec3 ghostPosition = m_GhostTile.getPosition();
    glm::vec3 position = glm::vec3{ ghostPosition.x, ghostPosition.y, 0.0f };
    m_TileManager->removeTile(position);
}
