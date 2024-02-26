#include "TimerManager.hpp"

#include "imgui.h"

#include "ImGuiUtility.hpp"

#include <chrono>
#include <iostream>

std::map<const char*, std::pair<timer_point, timer_point>> TimerManager::s_Timers;

void TimerManager::startTimer(const char* name)
{
    s_Timers[name] = std::make_pair(timer_clock::now(), timer_clock::now());
}

void TimerManager::endTimer(const char* name) { s_Timers[name].second = timer_clock::now(); }

void TimerManager::renderImGui()
{
    if (ImGui::Begin("Timers"))
    {
        std::map<const char*, double> times = calculateSeconds();

        for (const auto& p : times)
        {
            ImGuiUtility::TextCentered(std::format("{}", p.first));
            ImGuiUtility::TextCentered(std::format("{:.5}ms", p.second));
            // ImGui::Text("%lfms", p.first, p.second);
        }
    }
    ImGui::End();
}

std::map<const char*, double> TimerManager::calculateSeconds()
{
    std::map<const char*, double> returnTimes;

    for (const auto& p : s_Timers)
    {
        timer_point start = p.second.first;
        timer_point end = p.second.second;

        double final =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / (float)1e6;

        returnTimes[p.first] = final;
    }

    return returnTimes;
}
