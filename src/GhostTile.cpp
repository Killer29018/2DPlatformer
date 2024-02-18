#include "GhostTile.hpp"

GhostTile::GhostTile() : Tile() {}

void GhostTile::render(Shader& shader)
{
    shader.setInt("u_Ghost", 1);
    Tile::render(shader);
}
