#include "GhostTile.hpp"

GhostTile::GhostTile() : Tile() {}
GhostTile::GhostTile(glm::vec3 position, glm::ivec2 size, TileType type)
    : Tile(position, size, type)
{
}

void GhostTile::render(Shader& shader)
{
    if (m_Type == TileType::NONE) return;

    shader.setInt("u_Ghost", 1);
    Tile::render(shader);
}
