#pragma once

#include "Player.hpp"
#include "Window.hpp"
#include "events/EventHandler.hpp"
#include "tiles/GhostTile.hpp"
#include "tiles/TileManager.hpp"

class MapManager : public EventObserver
{
  public:
    MapManager();
    MapManager(Player* player, TileManager* tiles, Window* window);

    void receiveEvent(const Event* event) override;

  private:
    void placeBlock();
    void removeBlock();

  private:
    Player* m_Player;
    TileManager* m_TileManager;
    Window* m_Window;

    float m_TimeElapsed = 0.0f;

    GhostTile m_GhostTile;
    Shader m_GhostShader;
};
