#pragma once

#include "EventHandler.hpp"
#include "imgui.h"

#include "Window.hpp"
#include <memory>

class ImGuiManager : public EventDispatcher, public EventObserver
{
  public:
    ImGuiManager();
    ImGuiManager(Window* window);

    void preRender();
    void receiveEvent(const Event* event) override;
    void postRender();

  private:
    void initImGui();
    void showDockspace(bool* open);
    void setupStyle();

  private:
    std::unique_ptr<Window> m_Window;

    ImGuiIO* m_IO;
};
