#pragma once

#include <iostream>
#include <optional>
#include <type_traits>

#include "resources/TextureMap.hpp"

template<typename T>
concept EnumType = std::is_enum<T>::value;

template<EnumType animation>
class Animation
{
  public:
    typedef std::pair<bool, animation> AnimReturn;
    typedef std::function<AnimReturn(animation, float)> AnimFunc;

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

    void setAnimation(animation anim) { m_CurrentAnimation = anim; }
    void addAnimationSequence(AnimFunc func);

    void update(float dt);

  private:
    void init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight);

  private:
    TextureMap m_TextureMap;
    const std::unordered_map<animation, glm::ivec2>* m_NameToVec;

    animation m_CurrentAnimation;

    std::vector<AnimFunc> m_Functions;
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
    m_Functions = other.m_Functions;
    m_CurrentAnimation = other.m_CurrentAnimation;

    other.m_NameToVec = nullptr;
    other.m_Functions.clear();
}

template<EnumType T>
Animation<T>& Animation<T>::operator=(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_NameToVec = other.m_NameToVec;
    m_Functions = other.m_Functions;
    m_CurrentAnimation = other.m_CurrentAnimation;

    other.m_NameToVec = nullptr;
    other.m_Functions.clear();

    return *this;
}

template<EnumType T>
void Animation<T>::addAnimationSequence(Animation::AnimFunc func)
{
    m_Functions.push_back(func);
}

template<EnumType T>
void Animation<T>::update(float dt)
{
    static float currentTime = 0.0f;
    currentTime += dt;

    for (auto func : m_Functions)
    {
        AnimReturn nextValue = func(m_CurrentAnimation, currentTime);
        if (nextValue.first)
        {
            currentTime = 0.0f;
            m_CurrentAnimation = nextValue.second;
            break;
        }
    }
}
