#pragma once

#include "resources/TextureMap.hpp"
#include <iostream>
#include <map>
#include <optional>
#include <type_traits>

template<typename T>
concept EnumType = std::is_enum<T>::value;

#define TEMPLATE template<EnumType state>
#define CLASS_DEF Animation<state>

TEMPLATE
class Animation
{
  public:
    typedef std::pair<bool, state> AnimReturn;
    typedef std::function<AnimReturn(state, float timeElapsed)> AnimFunc;

    Animation() {}

    Animation(Animation& other) = delete;
    Animation(Animation&& other);

    Animation(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight,
              const std::unordered_map<state, std::vector<glm::ivec2>>* stateToFrames);

    Animation& operator=(Animation& other) = delete;
    Animation& operator=(Animation&& other);

    void bindTexture() { m_TextureMap.bind(); }
    glm::ivec2 getDimensions() { return m_TextureMap.getTileDimensions(); }

    glm::ivec2 getIndex() { return m_StateToFrames->at(m_CurrentState).at(m_CurrentFrameIndex); };

    void setState(state anim) { m_CurrentState = anim; }

    void addAnimationSequence(state animationState, float animationTime);

    void addConditionalTransition(const std::vector<state>& current, state to,
                                  std::function<bool(state, float)> predicate);

    void addConditionalTransition(state current, state to,
                                  std::function<bool(state, float)> predicate);

    void update(float dt);

  private:
    void init(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight);

  private:
    TextureMap m_TextureMap;

    const std::unordered_map<state, std::vector<glm::ivec2>>* m_StateToFrames;

    state m_CurrentState;
    size_t m_CurrentFrameIndex = 0;

    std::unordered_map<state, float> m_FrameTimes;
    std::multimap<state, AnimFunc> m_Transitions;
};

/* Definitions */

TEMPLATE
CLASS_DEF::Animation(const char* texturePath, uint32_t tileWidth, uint32_t tileHeight,
                     const std::unordered_map<state, std::vector<glm::ivec2>>* stateToFrames)
    : m_StateToFrames(stateToFrames)
{
    init(texturePath, tileWidth, tileHeight);
}

TEMPLATE
CLASS_DEF::Animation(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_StateToFrames = std::move(other.m_StateToFrames);
    m_FrameTimes = std::move(other.m_FrameTimes);
    m_Transitions = std::move(other.m_Transitions);

    m_CurrentState = other.m_CurrentState;
    m_CurrentFrameIndex = other.m_CurrentFrameIndex;

    other.m_NameToVec = nullptr;
    other.m_Animations.clear();
}

TEMPLATE
CLASS_DEF& CLASS_DEF::operator=(Animation&& other)
{
    m_TextureMap = std::move(other.m_TextureMap);
    m_StateToFrames = std::move(other.m_StateToFrames);
    m_FrameTimes = std::move(other.m_FrameTimes);
    m_Transitions = std::move(other.m_Transitions);

    m_CurrentState = other.m_CurrentState;
    m_CurrentFrameIndex = other.m_CurrentFrameIndex;

    return *this;
}

TEMPLATE
void CLASS_DEF::addAnimationSequence(state animationState, float animationTime)
{
    m_FrameTimes[animationState] = animationTime;
}

TEMPLATE
void CLASS_DEF::addConditionalTransition(const std::vector<state>& current, state to,
                                         std::function<bool(state, float)> predicate)
{
    for (auto i : current)
    {
        addConditionalTransition(i, to, predicate);
    }
}

TEMPLATE
void CLASS_DEF::addConditionalTransition(
    state current, state to, std::function<bool(state current, float timeElapsed)> predicate)
{
    m_Transitions.insert(std::make_pair(current, [predicate, to](state current, float timeElapsed) {
        if (predicate(current, timeElapsed))
            return std::make_pair(true, to);
        else
            return std::make_pair(false, current);
    }));
}

TEMPLATE
void CLASS_DEF::update(float dt)
{
    static float frameTime = 0.0f;
    static float currentTime = 0.0f;
    currentTime += dt;
    frameTime += dt;

    if (frameTime > m_FrameTimes.at(m_CurrentState))
    {
        m_CurrentFrameIndex =
            (m_CurrentFrameIndex + 1) % m_StateToFrames->at(m_CurrentState).size();

        frameTime = 0.0f;
    }

    auto range = m_Transitions.equal_range(m_CurrentState);
    for (auto it = range.first; it != range.second; it++)
    {
        AnimReturn nextValue = it->second(m_CurrentState, currentTime);
        if (nextValue.first)
        {
            currentTime = 0.0f;
            m_CurrentState = nextValue.second;
            m_CurrentFrameIndex = 0;
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
