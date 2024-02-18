#pragma once

#include "Tile.hpp"

class GhostTile : public Tile
{
  public:
    GhostTile();

    void render(Shader& shader);
};
