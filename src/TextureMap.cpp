#include "TextureMap.hpp"
#include "glad/gl.h"
#include <GL/gl.h>

uint32_t TextureMap::getID() { return m_Texture.getID(); }

void TextureMap::compileFromPath(const char* filename, uint32_t rows, uint32_t cols)
{
    m_Rows = rows;
    m_Cols = cols;

    m_Texture.setWrap(GL_REPEAT, GL_REPEAT);
    m_Texture.setFilters(GL_NEAREST, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 6);

    m_Texture.compileFromPath(filename);
}

void TextureMap::bind()
{
    m_Texture.bind();
    m_Texture.activeTexture(GL_TEXTURE0);
}

void TextureMap::unbind() { Texture2D::unbind(); }

glm::ivec2 TextureMap::getSize() { return m_Texture.getSize(); }
glm::ivec2 TextureMap::getTileSize()
{
    glm::ivec2 entireSize = getSize();
    return glm::ivec2{ entireSize.x / m_Cols, entireSize.y / m_Rows };
}
