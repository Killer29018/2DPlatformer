#pragma once

#include "Tile.hpp"

class GhostTile : public Tile
{
  public:
    GhostTile();
    GhostTile(glm::vec3 position, glm::ivec2 size, TileType type);

    void render(Shader& shader);
};
