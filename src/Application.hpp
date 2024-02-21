#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "ImGuiManager.hpp"
#include "MapManager.hpp"
#include "Player.hpp"
#include "SaveManager.hpp"
#include "Shader.hpp"
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
