#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Player.hpp"
#include "Shader.hpp"
#include "TextureMap.hpp"
#include "TileManager.hpp"
#include "Window.hpp"

class Application : public EventDispatcher, public EventObserver
{
  public:
    Application(glm::ivec2 windowSize, const char* title);
    ~Application();

    void start();

    void receiveEvent(const Event* event) override;

  private:
    TileManager m_Tiles;
    Player m_Player;

    Window m_Window;

    Shader m_Shader;
    TextureMap m_Texture;
    Camera m_Camera;

  private:
    void initialize();
    void mainLoop();
};
