#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

enum class PlayerAnimations {
    INVALID = 0,
    IDLE_1 = 1 << 0,
    IDLE_2 = 1 << 1,
    IDLE_3 = 1 << 2,
    IDLE_4 = 1 << 3,
    IDLE = IDLE_1 | IDLE_2 | IDLE_3 | IDLE_4,

    RUN_RIGHT_1 = 1 << 4,
    RUN_RIGHT_2 = 1 << 5,
    RUN_RIGHT_3 = 1 << 6,
    RUN_RIGHT_4 = 1 << 7,
    RUN_RIGHT = RUN_RIGHT_1 | RUN_RIGHT_2 | RUN_RIGHT_3 | RUN_RIGHT_4,

    RUN_LEFT_1 = 1 << 8,
    RUN_LEFT_2 = 1 << 9,
    RUN_LEFT_3 = 1 << 10,
    RUN_LEFT_4 = 1 << 11,
    RUN_LEFT = RUN_LEFT_1 | RUN_LEFT_2 | RUN_LEFT_3 | RUN_LEFT_4,

    RUNNING = RUN_RIGHT | RUN_LEFT,
};

const std::unordered_map<PlayerAnimations, glm::ivec2> PlayerAnimationToVec = {
    {PlayerAnimations::INVALID,      { -1, -1 }},
    { PlayerAnimations::IDLE_1,      { 0, 0 }  },
    { PlayerAnimations::IDLE_2,      { 1, 0 }  },
    { PlayerAnimations::IDLE_3,      { 2, 0 }  },
    { PlayerAnimations::IDLE_4,      { 3, 0 }  },

    { PlayerAnimations::RUN_RIGHT_1, { 0, 1 }  },
    { PlayerAnimations::RUN_RIGHT_2, { 1, 1 }  },
    { PlayerAnimations::RUN_RIGHT_3, { 2, 1 }  },
    { PlayerAnimations::RUN_RIGHT_4, { 3, 1 }  },

    { PlayerAnimations::RUN_LEFT_1,  { 0, 2 }  },
    { PlayerAnimations::RUN_LEFT_2,  { 1, 2 }  },
    { PlayerAnimations::RUN_LEFT_3,  { 2, 2 }  },
    { PlayerAnimations::RUN_LEFT_4,  { 3, 2 }  },
};

// struct PlayerAnimationCmp {
//     bool operator()(PlayerAnimations lhs, PlayerAnimations rhs) const
//     {
//         // lhs and rhs share no bits, just return the least
//         if ((static_cast<int>(lhs) & static_cast<int>(rhs)) == 0)
//             return static_cast<int>(lhs) < static_cast<int>(rhs);
//
//         // lhs and rhs share a bit, should be "equal"
//         return false;
//     }
// };
