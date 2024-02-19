#include "GhostTile.hpp"

GhostTile::GhostTile() : Tile() {}
GhostTile::GhostTile(glm::vec3 position, glm::ivec2 size, TileType type)
    : Tile(position, size, type)
{
}

void GhostTile::render(Shader& shader)
{
    shader.setInt("u_Ghost", 1);
    Tile::render(shader);
}
