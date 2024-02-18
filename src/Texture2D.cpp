#include "Texture2D.hpp"

#include <iostream>
#include <stdexcept>

Texture2D::Texture2D() {}

Texture2D::Texture2D(Texture2D&& other) : m_ID(other.m_ID) { other.m_ID.reset(); }

Texture2D::~Texture2D()
{
    try
    {
        uint32_t id = getID();
        glDeleteTextures(1, &id);
    } catch (texture_existence_error e)
    {
    }
}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    m_ID = std::move(other.m_ID);

    other.m_ID.reset();

    return *this;
}

uint32_t Texture2D::getID() const
{
    if (m_ID)
    {
        return m_ID.value();
    }
    else
    {
        throw texture_existence_error();
    }
}

void Texture2D::compileFromPath(const char* filename)
{
    int width;
    int height;
    int channels;

    uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data)
    {
        m_Channels = channels;
        m_Size = glm::ivec2{ width, height };

        uint32_t id;
        glGenTextures(1, &id);
        m_ID = id;
        glBindTexture(GL_TEXTURE_2D, getID());

        uint32_t imageFormat = GL_RED;
        if (m_Channels == 3)
            imageFormat = GL_RGB;
        else if (m_Channels == 4)
            imageFormat = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, imageFormat,
                     GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MagFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }

    stbi_image_free(data);
}

void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, getID()); }
void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
