#include "TextureMap.hpp"

#include "glad/gl.h"
#include <GL/gl.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <format>
#include <iostream>

#include "Texture2D.hpp"

uint32_t TextureMap::getID()
{
    if (m_ID)
        return m_ID.value();
    else
        throw texture_existence_error();
}

void TextureMap::compileFromPath(const char* filename, uint32_t rows, uint32_t cols)
{
    m_TileDimensions = glm::ivec2{ cols, rows };

    int width;
    int height;
    int channels;

    uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data)
    {
        m_Channels = channels;
        m_TotalSize = glm::ivec2{ width, height };
        m_TileSize = m_TotalSize / m_TileDimensions;

        std::cout << std::format("Total Size: {} : {}\n", m_TotalSize.x, m_TotalSize.y);
        std::cout << std::format("Dimensions: {} : {}\n", m_TileDimensions.x, m_TileDimensions.y);
        std::cout << std::format("Tile Size: {} : {}\n", m_TileSize.x, m_TileSize.y);

        uint32_t id;
        glGenTextures(1, &id);
        m_ID = id;

        glBindTexture(GL_TEXTURE_2D_ARRAY, getID());

        int imageCount = m_TileDimensions.x * m_TileDimensions.y;

        uint32_t imageFormat = GL_RED;
        if (m_Channels == 3)
            imageFormat = GL_RGB;
        else if (m_Channels == 4)
            imageFormat = GL_RGBA;

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, m_InternalFormat, m_TileSize.x, m_TileSize.y,
                     imageCount, 0, imageFormat, GL_UNSIGNED_BYTE, nullptr);

        std::vector<uint8_t> tile(m_TileSize.x * m_TileSize.y * channels);

        int tileSizeXBytes = m_TileSize.x * channels;
        int rowLen = m_TileDimensions.x * tileSizeXBytes;

        for (int y = 0; y < m_TileDimensions.y; y++)
        {
            for (int x = 0; x < m_TileDimensions.x; x++)
            {
                uint8_t* ptr = data + y * rowLen * m_TileSize.y + x * tileSizeXBytes;
                for (int row = 0; row < m_TileSize.y; row++)
                {
                    std::copy(ptr + row * rowLen, ptr + row * rowLen + tileSizeXBytes,
                              tile.begin() + row * tileSizeXBytes);
                }

                int i = y * m_TileDimensions.x + x;
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, m_TileSize.x, m_TileSize.y, 1,
                                imageFormat, GL_UNSIGNED_BYTE, tile.data());
            }
        }

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }

    stbi_image_free(data);
}

void TextureMap::bind() { glBindTexture(GL_TEXTURE_2D_ARRAY, getID()); }

glm::ivec2 TextureMap::getTileSize() { return m_TileSize; }
glm::ivec2 TextureMap::getTileDimensions() { return m_TileDimensions; }
glm::ivec2 TextureMap::getTotalSize() { return m_TotalSize; }

void TextureMap::setWrap(uint32_t wrapS, uint32_t wrapT)
{
    m_WrapS = wrapS;
    m_WrapT = wrapT;
}

void TextureMap::setFilters(uint32_t min, uint32_t mag)
{
    m_MinFilter = min;
    m_MagFilter = mag;
}

void TextureMap::setInternalFormat(uint32_t format) { m_InternalFormat = format; }
