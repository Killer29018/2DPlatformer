#pragma once

#include "Camera.hpp"
#include "ImGuiManager.hpp"
#include "MapManager.hpp"
#include "Player.hpp"
#include "SaveManager.hpp"
#include "Window.hpp"
#include "events/EventHandler.hpp"
#include "resources/Shader.hpp"
#include "tiles/TileManager.hpp"

class Application : public EventDispatcher, public EventObserver
{
  public:
    Application(glm::ivec2 windowSize, const char* title);
    ~Application();

    void start();

    void receiveEvent(const Event* event) override;

  private:
    Player m_Player;
    TileManager m_TileManager;
    MapManager m_MapManager;

    Window m_Window;

    Shader m_Shader;
    Camera m_Camera;

    ImGuiManager m_ImGuiManager;

    SaveManager m_SaveManager;

    float m_PreviousDT;

  private:
    void initialize();
    void mainLoop();
};
