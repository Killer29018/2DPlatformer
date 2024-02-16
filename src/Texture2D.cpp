#include "Texture2D.hpp"

#include <stdexcept>

uint32_t Texture2D::getID()
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

        if (m_Channels) printf("ALpha\n");

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, imageFormat,
                     GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }

    stbi_image_free(data);
}

void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, getID()); }
void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
