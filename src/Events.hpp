#pragma once

#include <cstdint>

class Camera;

enum class EventType : std::size_t {
    Event,
    Update,
    Render,
    KeyboardPress,
    MouseMove,
    MouseEnter,
    MousePress,
    MouseRelease,
    MouseDragged,
    WindowResize,
    ImGuiRender
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
    double xPosition;
    double yPosition;
    double xPercent;
    double yPercent;

  public:
    virtual EventType getType() const { return type; }
};

struct MouseEnterEvent : public Event {
  public:
    static constexpr EventType type = EventType::MouseEnter;

    bool entered;

  public:
    virtual EventType getType() const { return type; }
};

struct MousePressEvent : public Event {
  public:
    static constexpr EventType type = EventType::MousePress;

    bool leftClick;
    bool rightClick;
    double xPosition;
    double yPosition;

  public:
    virtual EventType getType() const { return type; }
};

struct MouseReleaseEvent : public Event {
  public:
    static constexpr EventType type = EventType::MouseRelease;

    bool leftClick;
    bool rightClick;
    double xPosition;
    double yPosition;

  public:
    virtual EventType getType() const { return type; }
};

struct MouseDraggedEvent : public Event {
  public:
    static constexpr EventType type = EventType::MouseDragged;

    bool leftClick;
    bool rightClick;
    double xPosition;
    double yPosition;
    double xOffset;
    double yOffset;

  public:
    virtual EventType getType() const { return type; }
};

struct WindowResizeEvent : public Event {
  public:
    static constexpr EventType type = EventType::WindowResize;

    int previousWidth;
    int previousHeight;
    int newWidth;
    int newHeight;

  public:
    virtual EventType getType() const { return type; }
};

struct ImGuiRenderEvent : public Event {
  public:
    static constexpr EventType type = EventType::ImGuiRender;

  public:
    virtual EventType getType() const { return type; }
};
