#pragma once

#include <type_traits>

#include "resources/TextureMap.hpp"

template<typename T>
concept EnumType = std::is_enum<T>::value;

template<EnumType animation>
class Animation
{
  public:
    Animation() {}

    Animation(Animation& other) = delete;
    Animation(Animation&& other);

    Animation(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight,
              const std::unordered_map<animation, glm::ivec2>* nameToVec);

    Animation& operator=(Animation& other) = delete;
    Animation& operator=(Animation&& other);

    void bindTexture() { m_TextureMap.bind(); }
    glm::ivec2 getDimensions() { return m_TextureMap.getTileDimensions(); }

    glm::ivec2 getIndex() { return m_NameToVec->at(m_CurrentAnimation); };

  private:
    void init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight);

  private:
    TextureMap m_TextureMap;
    const std::unordered_map<animation, glm::ivec2>* m_NameToVec;

    animation m_CurrentAnimation;
};

/* Definitions */

template<EnumType T>
Animation<T>::Animation(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight,
                        const std::unordered_map<T, glm::ivec2>* nameToVec)
    : m_NameToVec(nameToVec)
{
    init(texturePath, tileWidth, tileHeight);
}

template<EnumType T>
void Animation<T>::init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight)
{
    m_TextureMap.compileFromPath(texturePath, tileWidth, tileHeight);
}

template<EnumType T>
Animation<T>::Animation(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_NameToVec = other.m_NameToVec;

    other.m_NameToVec = nullptr;
}

template<EnumType T>
Animation<T>& Animation<T>::operator=(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_NameToVec = other.m_NameToVec;

    other.m_NameToVec = nullptr;

    return *this;
}
