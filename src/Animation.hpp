#pragma once

#include <iostream>
#include <optional>
#include <type_traits>

#include <map>

#include "resources/TextureMap.hpp"

template<typename T>
concept EnumType = std::is_enum<T>::value;

#define TEMPLATE template<EnumType animation>
#define CLASS_DEF Animation<animation>

TEMPLATE
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
    void addAnimationSequence(const std::vector<animation>& animations, float animationTime);
    void addConditionalTransition(animation current, animation to,
                                  std::function<bool(animation, float)> predicate);
    void addConditionalTransition(const std::vector<animation>& current, animation to,
                                  std::function<bool(animation, float)> predicate);

    void update(float dt);

  private:
    void init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight);

  private:
    TextureMap m_TextureMap;
    const std::unordered_map<animation, glm::ivec2>* m_NameToVec;

    animation m_CurrentAnimation;

    std::multimap<animation, AnimFunc> m_Animations;
};

/* Definitions */

TEMPLATE
CLASS_DEF::Animation(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight,
                     const std::unordered_map<animation, glm::ivec2>* nameToVec)
    : m_NameToVec(nameToVec)
{
    init(texturePath, tileWidth, tileHeight);
}

TEMPLATE
CLASS_DEF::Animation(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_NameToVec = other.m_NameToVec;
    m_Animations = other.m_Animations;
    m_CurrentAnimation = other.m_CurrentAnimation;

    other.m_NameToVec = nullptr;
    other.m_Animations.clear();
}

TEMPLATE
CLASS_DEF& CLASS_DEF::operator=(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_NameToVec = other.m_NameToVec;
    m_Animations = other.m_Animations;
    m_CurrentAnimation = other.m_CurrentAnimation;

    other.m_NameToVec = nullptr;
    other.m_Animations.clear();

    return *this;
}

TEMPLATE
void CLASS_DEF::addAnimationSequence(const std::vector<animation>& animations, float animationTime)
{
    for (size_t i = 0; i < animations.size(); i++)
    {
        animation current = animations.at(i);
        animation next = animations.at((i + 1) % animations.size());

        m_Animations.insert(
            std::make_pair(current, [next, animationTime](animation current, float timeElapsed) {
                if (timeElapsed < animationTime) return std::make_pair(false, current);

                return std::make_pair(true, next);
            }));
    }
}

TEMPLATE
void CLASS_DEF::addConditionalTransition(
    const std::vector<animation>& current, animation to,
    std::function<bool(animation current, float timeElapsed)> predicate)
{
    for (auto i : current)
    {
        addConditionalTransition(i, to, predicate);
    }
}

TEMPLATE
void CLASS_DEF::addConditionalTransition(
    animation current, animation to,
    std::function<bool(animation current, float timeElapsed)> predicate)
{
    // Decompose current into bits
    int mask = 1;
    int intValue = static_cast<int>(current);
    while (intValue != 0)
    {
        if ((intValue & mask) != 0)
        {
            animation splitAnimation = static_cast<animation>(intValue & mask);
            m_Animations.insert(std::make_pair(
                splitAnimation, [predicate, to](animation current, float timeElapsed) {
                    if (predicate(current, timeElapsed))
                        return std::make_pair(true, to);
                    else
                        return std::make_pair(false, current);
                }));
        }
        intValue &= (~mask);
        mask <<= 1;
    }
}

TEMPLATE
void CLASS_DEF::update(float dt)
{
    static float currentTime = 0.0f;
    currentTime += dt;

    auto range = m_Animations.equal_range(m_CurrentAnimation);
    for (auto it = range.first; it != range.second; it++)
    {
        AnimReturn nextValue = it->second(m_CurrentAnimation, currentTime);
        if (nextValue.first)
        {
            currentTime = 0.0f;
            m_CurrentAnimation = nextValue.second;
            break;
        }
    }
}

TEMPLATE
void CLASS_DEF::init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight)
{
    m_TextureMap.compileFromPath(texturePath, tileWidth, tileHeight);
}

#undef CLAS_DEFS
#undef TEMPLATE
