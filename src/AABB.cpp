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

glm::vec4 AABB::resolveBoundingBox(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB,
                                   glm::vec2 velocityA)
{
    // float xInvEntry, yInvEntry;
    // float xInvExit, yInvExit;
    //
    // if (velocityA.x > 0.0f)
    // {
    //     xInvEntry = boundingBoxB.x - (boundingBoxA.x + boundingBoxA.z);
    //     xInvExit = (boundingBoxB.x + boundingBoxB.z) - boundingBoxA.x;
    // }
    // else
    // {
    //     xInvEntry = (boundingBoxB.x + boundingBoxB.z) - boundingBoxA.x;
    //     xInvExit = boundingBoxB.x - (boundingBoxA.x + boundingBoxA.z);
    // }
    //
    // if (velocityA.y < 0.0f)
    // {
    //     yInvEntry = (boundingBoxB.y + boundingBoxB.w) - boundingBoxA.y;
    //     yInvExit = boundingBoxB.y - (boundingBoxA.y + boundingBoxA.w);
    // }
    // else
    // {
    //     yInvEntry = boundingBoxB.y - (boundingBoxA.y + boundingBoxA.w);
    //     yInvExit = (boundingBoxB.y + boundingBoxB.w) - boundingBoxA.y;
    // }
    //
    // float xEntry, yEntry;
    // float xExit, yExit;
    //
    // if (velocityA.x == 0.0f)
    // {
    //     xEntry = -std::numeric_limits<float>::infinity() + 1;
    //     xExit = std::numeric_limits<float>::infinity() - 1;
    // }
    // else
    // {
    //     xEntry = xInvEntry / velocityA.x;
    //     xExit = xInvExit / velocityA.x;
    // }
    //
    // if (velocityA.y == 0.0f)
    // {
    //     yEntry = -std::numeric_limits<float>::infinity() + 1;
    //     yExit = std::numeric_limits<float>::infinity() - 1;
    // }
    // else
    // {
    //     yEntry = yInvEntry / velocityA.y;
    //     yExit = yInvExit / velocityA.y;
    // }
    //
    // float entryTime = std::max(xEntry, yEntry);
    // float exitTime = std::min(xExit, yExit);
    //
    // float normalX, normalY;
    //
    // printf("xEntry: %f, yEntry: %f\n", xEntry, yEntry);
    // if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry
    // > 1.0f)
    // {
    //     return glm::vec4{ boundingBoxA.x + velocityA.x, boundingBoxA.y + velocityA.y,
    //     velocityA.x,
    //                       velocityA.y };
    // }
    // else
    // {
    //     if (xEntry > yEntry)
    //     {
    //         if (xInvEntry < 0.0f)
    //         {
    //             normalX = 1.0f;
    //             normalY = 0.0f;
    //         }
    //         else
    //         {
    //             normalX = -1.0f;
    //             normalY = 0.0f;
    //         }
    //     }
    //     else
    //     {
    //         if (yInvEntry < 0.0f)
    //         {
    //             normalX = 0.0f;
    //             normalY = -1.0f;
    //         }
    //         else
    //         {
    //             normalX = 0.0f;
    //             normalY = 1.0f;
    //         }
    //     }
    // }
    //
    // printf("Entry Time: %f\n", entryTime);
    // printf("xEntry: %f, yEntry: %f\n", xEntry, yEntry);
    //
    // float remainingTime = 1.0f - entryTime;
    //
    // float dotprod = (velocityA.x * normalY + velocityA.y * normalX) * remainingTime;
    //
    // float collisionTime = entryTime;
    // glm::vec2 position = glm::vec2{ boundingBoxA.x + velocityA.x * collisionTime,
    //                                 boundingBoxA.y + velocityA.y * collisionTime };
    //
    // velocityA.x = dotprod * normalY;
    // velocityA.y = dotprod * normalX;
    //
    // return glm::vec4{ position.x, position.y, velocityA.x, velocityA.y };
}
