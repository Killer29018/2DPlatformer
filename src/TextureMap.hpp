#pragma once

#include "Texture2D.hpp"

class TextureMap
{
  public:
    TextureMap() {}

    uint32_t getID();

    void compileFromPath(const char* filename, uint32_t rows, uint32_t cols);

    void bind();
    static void unbind();

    static void activeTexture(uint32_t texture) { glActiveTexture(texture); }

    glm::ivec2 getSize();
    glm::ivec2 getDimensions();
    glm::ivec2 getTileSize();

  private:
    Texture2D m_Texture;

    uint32_t m_Rows, m_Cols;
};
