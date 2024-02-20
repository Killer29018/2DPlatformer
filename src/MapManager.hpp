#pragma once

#include "EventHandler.hpp"
#include "GhostTile.hpp"
#include "Player.hpp"
#include "TileManager.hpp"
#include "Window.hpp"

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

    GhostTile m_GhostTile;
    Shader m_GhostShader;
};