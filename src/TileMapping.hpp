#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <map>

enum class TileMap : int32_t {
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

const std::map<TileMap, glm::ivec2> TileMapToVec = {
    {                  TileMap::NONE, { -1, -1 }},
    {        TileMap::STONE_TOP_LEFT,   { 0, 0 }},
    {      TileMap::STONE_TOP_CENTER,   { 1, 0 }},
    {       TileMap::STONE_TOP_RIGHT,   { 2, 0 }},

    {     TileMap::STONE_CENTER_LEFT,   { 0, 1 }},
    {          TileMap::STONE_CENTER,   { 1, 1 }},
    {    TileMap::STONE_CENTER_RIGHT,   { 2, 1 }},

    {     TileMap::STONE_BOTTOM_LEFT,   { 0, 2 }},
    {   TileMap::STONE_BOTTOM_CENTER,   { 1, 2 }},
    {    TileMap::STONE_BOTTOM_RIGHT,   { 2, 2 }},

    {  TileMap::STONE_GRASS_TOP_LEFT,   { 3, 1 }},
    {TileMap::STONE_GRASS_TOP_CENTER,   { 4, 1 }},
    { TileMap::STONE_GRASS_TOP_RIGHT,   { 5, 1 }},

    {  TileMap::ABOVE_GRASS_TOP_LEFT,   { 3, 0 }},
    {TileMap::ABOVE_GRASS_TOP_CENTER,   { 4, 0 }},
    { TileMap::ABOVE_GRASS_TOP_RIGHT,   { 5, 0 }},
};
