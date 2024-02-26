#pragma once

#include "events/EventHandler.hpp"
#include <chrono>

typedef std::chrono::steady_clock timer_clock;
typedef std::chrono::time_point<timer_clock> timer_point;

class TimerManager : public EventDispatcher, public EventObserver
{
  public:
    static void startTimer(const char* name);
    static void endTimer(const char* name);

    static void renderImGui();

  private:
    static std::map<const char*, double> calculateSeconds();

  private:
    static std::map<const char*, std::pair<timer_point, timer_point>> s_Timers;
};
