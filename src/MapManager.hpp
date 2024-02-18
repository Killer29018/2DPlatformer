#pragma once

#include "EventHandler.hpp"
#include "GhostTile.hpp"
#include "Player.hpp"
#include "TileManager.hpp"

class MapManager : public EventObserver
{
  public:
    MapManager();
    MapManager(Player* player, TileManager* tiles);

    void receiveEvent(const Event* event) override;

  private:
    Player* m_Player;
    TileManager* m_TileManager;

    GhostTile m_GhostTile;
    Shader m_GhostShader;
};
