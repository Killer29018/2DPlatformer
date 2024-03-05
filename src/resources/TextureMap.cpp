#include "TextureMap.hpp"

#include "glad/gl.h"
#include <GL/gl.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <format>
#include <iostream>

#include "Texture2D.hpp"

TextureMap::TextureMap() {}

TextureMap::TextureMap(TextureMap&& other) : m_ID(other.m_ID)
{
    other.m_ID.reset();
    m_EntireTexture = std::move(other.m_EntireTexture);

    m_TileDimensions = other.m_TileDimensions;
    m_TileSize = other.m_TileSize;
    m_TotalSize = other.m_TotalSize;
}

TextureMap::~TextureMap()
{
    try
    {
        uint32_t id = getID();
        glDeleteTextures(1, &id);
    } catch (texture_existence_error e)
    {
    }
}

TextureMap& TextureMap::operator=(TextureMap&& other)
{
    m_ID = other.m_ID;
    m_EntireTexture = std::move(other.m_EntireTexture);
    m_TileDimensions = other.m_TileDimensions;
    m_TileSize = other.m_TileSize;
    m_TotalSize = other.m_TotalSize;

    other.m_ID.reset();

    return *this;
}

uint32_t TextureMap::getID() const
{
    if (m_ID)
        return m_ID.value();
    else
        throw texture_existence_error();
}

void TextureMap::compileFromPath(const char* filename, uint32_t tileWidth, uint32_t tileHeight)
{
    m_TileSize = glm::ivec2{ tileWidth, tileHeight };

    int width;
    int height;
    int channels;

    uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data)
    {
        m_Channels = channels;
        m_TotalSize = glm::ivec2{ width, height };
        m_TileDimensions = m_TotalSize / m_TileSize;

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

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, m_MinFilter);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, m_MagFilter);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, m_WrapS);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, m_WrapT);

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }

    stbi_image_free(data);

    m_EntireTexture.compileFromPath(filename);
}

void TextureMap::bind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, getID()); }

glm::ivec2 TextureMap::getTileSize() const { return m_TileSize; }
glm::ivec2 TextureMap::getTileDimensions() const { return m_TileDimensions; }
glm::ivec2 TextureMap::getTotalSize() const { return m_TotalSize; }
const Texture2D& TextureMap::getEntireTexture() const { return m_EntireTexture; }

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
