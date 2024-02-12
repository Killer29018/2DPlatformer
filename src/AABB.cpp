#include "AABB.hpp"
#include <cstdio>
#include <limits>
#include <type_traits>

bool AABB::collision(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB)
{
    return (boundingBoxA.x < (boundingBoxB.x + boundingBoxB.z) &&
            (boundingBoxA.x + boundingBoxA.z) > boundingBoxB.x &&
            boundingBoxA.y < (boundingBoxB.y + boundingBoxB.w) &&
            (boundingBoxA.y + boundingBoxA.w) > boundingBoxB.y);
}

glm::vec2 AABB::calculateOffsets(glm::vec4 bbA, glm::vec4 bbB, glm::vec2 vA)
{
    float xOffset, yOffset;

    if (vA.x < 0)
    {
        xOffset = (bbB.x + bbB.z) - bbA.x;
    }
    else
    {
        xOffset = bbB.x - (bbA.x + bbA.z);
    }

    if (vA.y < 0)
    {
        yOffset = (bbB.y + bbB.w) - (bbA.y);
    }
    else
    {
        yOffset = -(bbA.y + bbA.w) + (bbB.y);
    }

    return glm::vec2{ xOffset, yOffset };
}
