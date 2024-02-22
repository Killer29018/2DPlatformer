#pragma once

#include "events/EventHandler.hpp"
#include "json/writer.h"

class SaveManager : public EventDispatcher
{
  public:
    SaveManager();

  public:
    void saveGame();
    void loadGame();
};
