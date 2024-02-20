#pragma once

#include "Texture2D.hpp"
#include "glad/gl.h"

class TextureMap
{
  public:
    TextureMap();
    TextureMap(TextureMap& other) = delete;
    TextureMap(TextureMap&& other);
    ~TextureMap();

    TextureMap& operator=(TextureMap&& other);

    uint32_t getID() const;

    void compileFromPath(const char* filename, uint32_t tileWidth, uint32_t tileHeight);

    void bind() const;
    static void unbind() { glBindTexture(GL_TEXTURE_3D, 0); }

    static void activeTexture(uint32_t texture) { glActiveTexture(texture); }

    glm::ivec2 getTileSize() const;
    glm::ivec2 getTileDimensions() const;
    glm::ivec2 getTotalSize() const;

    const Texture2D& getEntireTexture() const;

    void setWrap(uint32_t wrapS, uint32_t wrapT);
    void setFilters(uint32_t min, uint32_t mag);
    void setInternalFormat(uint32_t format);

  private:
    std::optional<uint32_t> m_ID;

    glm::ivec2 m_TileDimensions;
    glm::ivec2 m_TileSize;
    glm::ivec2 m_TotalSize;

    uint32_t m_Channels;

    uint32_t m_WrapS = GL_CLAMP_TO_EDGE;
    uint32_t m_WrapT = GL_CLAMP_TO_EDGE;

    uint32_t m_MinFilter = GL_NEAREST;
    uint32_t m_MagFilter = GL_NEAREST;

    uint32_t m_InternalFormat = GL_RGBA;

    Texture2D m_EntireTexture;
};
