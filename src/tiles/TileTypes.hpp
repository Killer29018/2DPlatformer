#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <map>

#include <algorithm>
#include <optional>

enum class TileType : int32_t {
    NONE = 0,
    STONE_TOP_LEFT,
    STONE_TOP_CENTER,
    STONE_TOP_RIGHT,
    STONE_CENTER_LEFT,
    STONE_CENTER,
    STONE_CENTER_RIGHT,
    STONE_BOTTOM_LEFT,
    STONE_BOTTOM_CENTER,
    STONE_BOTTOM_RIGHT,

    STONE_GRASS_TOP_LEFT,
    STONE_GRASS_TOP_CENTER,
    STONE_GRASS_TOP_RIGHT,

    ABOVE_GRASS_TOP_LEFT,
    ABOVE_GRASS_TOP_CENTER,
    ABOVE_GRASS_TOP_RIGHT,

    LAST
};

constexpr size_t MAX_TILE_VEC = 3;

const std::map<TileType, std::vector<glm::ivec2>> TileTypeToVec = {
    {TileType::NONE,                    { { -1, -1 } }        },
    { TileType::STONE_TOP_LEFT,         { { 0, 0 } }          },
    { TileType::STONE_TOP_CENTER,       { { 1, 0 } }          },
    { TileType::STONE_TOP_RIGHT,        { { 2, 0 } }          },

    { TileType::STONE_CENTER_LEFT,      { { 0, 1 } }          },
    { TileType::STONE_CENTER,           { { 1, 1 }, { 1, 3 } }},
    { TileType::STONE_CENTER_RIGHT,     { { 2, 1 } }          },

    { TileType::STONE_BOTTOM_LEFT,      { { 0, 2 } }          },
    { TileType::STONE_BOTTOM_CENTER,    { { 1, 2 } }          },
    { TileType::STONE_BOTTOM_RIGHT,     { { 2, 2 } }          },

    { TileType::STONE_GRASS_TOP_LEFT,   { { 3, 1 } }          },
    { TileType::STONE_GRASS_TOP_CENTER, { { 4, 1 } }          },
    { TileType::STONE_GRASS_TOP_RIGHT,  { { 5, 1 } }          },

    { TileType::ABOVE_GRASS_TOP_LEFT,   { { 3, 0 } }          },
    { TileType::ABOVE_GRASS_TOP_CENTER, { { 4, 0 } }          },
    { TileType::ABOVE_GRASS_TOP_RIGHT,  { { 5, 0 } }          },
};

inline std::optional<TileType> VecToTileType(glm::ivec2 coord)
{
    auto result = std::find_if(TileTypeToVec.begin(), TileTypeToVec.end(),
                               [&](const std::pair<TileType, std::vector<glm::ivec2>>& pair) {
                                   auto result = std::find_if(
                                       pair.second.begin(), pair.second.end(),
                                       [&](glm::ivec2 value) { return value == coord; });

                                   return (result != pair.second.end());
                               });

    std::optional<TileType> returnValue;
    if (result != TileTypeToVec.end()) returnValue = result->first;

    return returnValue;
}
