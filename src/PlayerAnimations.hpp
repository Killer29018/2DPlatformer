#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#define INCREMENT(x) x << 1

enum class PlayerAnimations {
    INVALID = 0,
    IDLE_1 = 1,
    IDLE_2 = INCREMENT(IDLE_1),
    IDLE_3 = INCREMENT(IDLE_2),
    IDLE_4 = INCREMENT(IDLE_3),
    IDLE = IDLE_1 | IDLE_2 | IDLE_3 | IDLE_4,

    RUN_RIGHT_1 = INCREMENT(IDLE_4),
    RUN_RIGHT_2 = INCREMENT(RUN_RIGHT_1),
    RUN_RIGHT_3 = INCREMENT(RUN_RIGHT_2),
    RUN_RIGHT_4 = INCREMENT(RUN_RIGHT_3),
    RUN_RIGHT = RUN_RIGHT_1 | RUN_RIGHT_2 | RUN_RIGHT_3 | RUN_RIGHT_4,

    RUN_LEFT_1 = INCREMENT(RUN_RIGHT_4),
    RUN_LEFT_2 = INCREMENT(RUN_LEFT_1),
    RUN_LEFT_3 = INCREMENT(RUN_LEFT_2),
    RUN_LEFT_4 = INCREMENT(RUN_LEFT_3),
    RUN_LEFT = RUN_LEFT_1 | RUN_LEFT_2 | RUN_LEFT_3 | RUN_LEFT_4,

    RUNNING = RUN_RIGHT | RUN_LEFT,
};

const std::unordered_map<PlayerAnimations, glm::ivec2> PlayerAnimationToVec = {
    {PlayerAnimations::INVALID,      { -1, -1 }},
    { PlayerAnimations::IDLE_1,      { 0, 0 }  },
    { PlayerAnimations::IDLE_2,      { 1, 0 }  },
    { PlayerAnimations::IDLE_3,      { 2, 0 }  },
    { PlayerAnimations::IDLE_4,      { 3, 0 }  },

    { PlayerAnimations::RUN_RIGHT_1, { 4, 0 }  },
    { PlayerAnimations::RUN_RIGHT_2, { 5, 0 }  },
    { PlayerAnimations::RUN_RIGHT_3, { 6, 0 }  },
    { PlayerAnimations::RUN_RIGHT_4, { 7, 0 }  },

    { PlayerAnimations::RUN_LEFT_1,  { 4, 0 }  },
    { PlayerAnimations::RUN_LEFT_2,  { 5, 0 }  },
    { PlayerAnimations::RUN_LEFT_3,  { 6, 0 }  },
    { PlayerAnimations::RUN_LEFT_4,  { 7, 0 }  },
};

#undef INCREMENT

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
