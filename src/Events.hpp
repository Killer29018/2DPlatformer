#pragma once

#include <cstdint>

class Camera;

enum class EventType : std::size_t {
    Event,
    Update,
    Render,
    KeyboardPress,
    MouseMove,
    MousePosition,
    MouseEnter
};

struct Event {
  public:
    static constexpr EventType type = EventType::Event;

    Event(){};

  public:
    virtual ~Event() {}
    virtual EventType getType() const { return type; }
};

struct UpdateEvent : public Event {
  public:
    static constexpr EventType type = EventType::Update;

    float dt;

  public:
    virtual EventType getType() const { return type; }
};

struct RenderEvent : public Event {
  public:
    static constexpr EventType type = EventType::Render;

    Camera* camera;

  public:
    virtual EventType getType() const { return type; }
};

struct KeyboardEvent : public Event {
  public:
    static constexpr EventType type = EventType::KeyboardPress;

    int key;
    int action;
    int scancode;
    int mods;

  public:
    virtual EventType getType() const { return type; }
};

struct MouseMoveEvent : public Event {
  public:
    static constexpr EventType type = EventType::MouseMove;

    double xOffset;
    double yOffset;

  public:
    virtual EventType getType() const { return type; }
};

struct MousePositionEvent : public Event {
  public:
    static constexpr EventType type = EventType::MousePosition;

    double xPosition;
    double yPosition;
    virtual EventType getType() const { return type; }
};

struct MouseEnterEvent : public Event {
  public:
    static constexpr EventType type = EventType::MouseEnter;

    bool entered;

  public:
    virtual EventType getType() const { return type; }
};
