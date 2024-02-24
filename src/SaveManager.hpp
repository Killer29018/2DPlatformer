#pragma once

#include "events/EventHandler.hpp"
#include "json/writer.h"

#include <filesystem>
#include <streambuf>
#include <vector>

class SaveManager : public EventDispatcher, public EventObserver
{
  public:
    SaveManager();

    void receiveEvent(const Event* event) override;

  public:
    void saveState(const char* name);
    void loadState(const char* name);

  private:
    void reloadStates();

    void saveStateToPath(std::filesystem::path path);
    void loadStateFromPath(std::filesystem::path path);

  private:
    const std::filesystem::path m_StateFolder = "saves/";
    const std::string m_StateSuffix = ".json";

    std::vector<std::filesystem::path> m_States;
};
