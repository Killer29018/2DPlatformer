#pragma once

#include "stb_image.h"
#include <cstdint>
#include <exception>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <optional>

class texture_existence_error : public std::exception
{
  public:
    texture_existence_error() {}

    const char* what() const noexcept { return "Texture does not exist"; }
};

class Texture2D
{
  public:
    Texture2D();
    Texture2D(Texture2D& other) = delete;
    Texture2D(Texture2D&& other);
    ~Texture2D();

    Texture2D& operator=(Texture2D&& other);

    uint32_t getID();

    void compileFromPath(const char* filename);

    void bind();
    static void unbind();

    static void activeTexture(uint32_t texture) { glActiveTexture(texture); }

    glm::ivec2 getSize() { return m_Size; }

    void setWrap(uint32_t wrapS, uint32_t wrapT)
    {
        m_WrapS = wrapS;
        m_WrapT = wrapT;
    }

    void setFilters(uint32_t min, uint32_t mag)
    {
        m_MinFilter = min;
        m_MagFilter = mag;
    }

    void setInternalFormat(uint32_t format) { m_InternalFormat = format; }

  private:
    std::optional<uint32_t> m_ID;

    uint32_t m_WrapS = GL_REPEAT;
    uint32_t m_WrapT = GL_REPEAT;

    uint32_t m_MinFilter = GL_LINEAR;
    uint32_t m_MagFilter = GL_LINEAR;

    uint32_t m_InternalFormat = GL_RGBA;

    glm::ivec2 m_Size;
    uint32_t m_Channels;

  private:
};
