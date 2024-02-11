#pragma once

#include <glm/glm.hpp>

class AABB
{
  public:
    static bool collision(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB);
    static glm::vec4 resolveBoundingBox(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB,
                                        glm::vec2 velocityA);
};
