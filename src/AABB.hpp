#pragma once

#include <glm/glm.hpp>

class AABB
{
  public:
    static bool collision(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB);
    static glm::vec2 calculateOffsets(glm::vec4 boundingBoxA, glm::vec4 boundingBoxB,
                                      glm::vec2 velocityA);
};
