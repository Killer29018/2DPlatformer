#include "SaveManager.hpp"
#include "events/Events.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <memory>
#include <sys/select.h>

#include "imgui.h"

SaveManager::SaveManager() {}

void SaveManager::receiveEvent(const Event* event)
{
    switch (event->getType())
    {
    case EventType::ImGuiRender:
        {
            const ImGuiRenderEvent* irEvent = reinterpret_cast<const ImGuiRenderEvent*>(event);

            if (ImGui::Begin("Save Manager"))
            {
                static int selectedIndex = -1;
                if (ImGui::Button("Refresh Saves"))

                {
                    // Reload saved list of saves
                    reloadStates();
                    selectedIndex = -1;
                }

                // Display saves and allow to be loaded
                if (ImGui::BeginTable("##SaveLoad", 2))
                {
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Load State") && selectedIndex <= (int)m_States.size() - 1)
                    {

                        std::filesystem::path savedPath = m_States.at(selectedIndex);
                        loadStateFromPath(savedPath);
                        reloadStates();

                        // Path may have moved after reload so refind index
                        auto it =
                            std::find_if(m_States.begin(), m_States.end(),
                                         [&](std::filesystem::path v) { return v == savedPath; });

                        selectedIndex = it - m_States.begin();
                        if (it == m_States.end()) selectedIndex = -1;
                    }

                    ImGui::NewLine();
                    ImGui::Text("Save Name: ");

                    constexpr size_t saveNameSize = 100;
                    static char saveName[saveNameSize] = "temporary";
                    ImGui::InputText("##SaveName", saveName, saveNameSize);

                    if (ImGui::Button("Save State"))
                    {
                        saveState(saveName);
                        reloadStates();

                        auto it = std::find_if(
                            m_States.begin(), m_States.end(), [&](std::filesystem::path v) {
                                return v.filename().stem().generic_string() == saveName;
                            });

                        selectedIndex = it - m_States.begin();
                        if (it == m_States.end()) selectedIndex = -1;
                    }

                    if (ImGui::Button("Delete State"))
                    {
                        std::filesystem::remove(m_States.at(selectedIndex));
                        reloadStates();
                        selectedIndex = 0;
                    }

                    // ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::BeginListBox("##Saves",
                                            ImVec2(-1, 10 * ImGui::GetTextLineHeightWithSpacing())))
                    {
                        for (int i = 0; i < (int)m_States.size(); i++)
                        {
                            const std::string filename = m_States[i].filename().generic_string();
                            const bool isSelected = (selectedIndex == i);
                            if (ImGui::Selectable(filename.c_str(), isSelected)) selectedIndex = i;

                            if (isSelected) ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndListBox();
                    }

                    ImGui::EndTable();
                }

                ImGui::End();
            }

            break;
        }
    default:
        break;
    }
}

void SaveManager::saveState(const char* name)
{
    std::filesystem::path saveLocation = m_StateFolder;
    saveLocation += name;
    saveLocation.replace_extension(m_StateSuffix);

    saveStateToPath(saveLocation);
}

void SaveManager::loadState(const char* name)
{
    std::filesystem::path saveLocation = m_StateFolder;
    saveLocation += name;
    saveLocation.replace_extension(m_StateSuffix);

    if (!std::filesystem::exists(saveLocation)) return;

    loadStateFromPath(saveLocation);
}

void SaveManager::reloadStates()
{
    m_States.clear();

    for (const auto& entry : std::filesystem::directory_iterator(m_StateFolder))
    {
        if (entry.path().extension() != m_StateSuffix)
        {
            continue;
        }

        std::string errs;
        Json::Value root;
        Json::CharReaderBuilder reader;
        reader["collectComments"] = false;

        std::ifstream input{ entry.path().generic_string(), std::ios::in };

        bool ok = Json::parseFromStream(reader, input, &root, &errs);

        if (!ok) continue;

        m_States.push_back(entry.path());
    }
}

void SaveManager::saveStateToPath(std::filesystem::path path)
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";

    Json::Value root;

    // Emit Event
    SaveGameEvent sgEvent;
    sgEvent.root = &root;

    notify(&sgEvent);

    std::ofstream output{ path.generic_string(), std::ios::in | std::ios::trunc };

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &output);

    output.close();
}

void SaveManager::loadStateFromPath(std::filesystem::path path)
{
    std::ifstream input{ path.generic_string(), std::ios::in };

    std::string errs;
    Json::Value root;
    Json::CharReaderBuilder reader;
    reader["collectComments"] = false;
    bool ok = Json::parseFromStream(reader, input, &root, &errs);

    if (!ok)
    {
        std::cout << "Failed to parse JSON: " << path.generic_string() << "\n";
        return;
    }

    input.close();

    LoadGameEvent lgEvent;
    lgEvent.root = &root;

    notify(&lgEvent);
}
