#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#define INCREMENT(x) x << 1

enum class PlayerAnimationState { IDLE, RUN_RIGHT, RUN_LEFT };

const std::unordered_map<PlayerAnimationState, std::vector<glm::ivec2>>
    PlayerAnimationStateToFrame = {
        {PlayerAnimationState::IDLE,       { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 0 } }},
        { PlayerAnimationState::RUN_RIGHT, { { 3, 0 }, { 4, 0 }, { 5, 0 }, { 4, 0 } }},
        { PlayerAnimationState::RUN_LEFT,  { { 3, 0 }, { 4, 0 }, { 5, 0 }, { 4, 0 } }}
};

#undef INCREMENT
