#include "SaveManager.hpp"

#include <fstream>
#include <iostream>

SaveManager::SaveManager() {}

void SaveManager::saveGame()
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";

    Json::Value root;

    // Emit Event
    SaveGameEvent sgEvent;
    sgEvent.root = &root;

    notify(&sgEvent);

    std::ofstream output{ "saves/data.json", std::ios::in | std::ios::trunc };

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &output);

    output.close();

    std::cout << "Saved\n";
}

void SaveManager::loadGame()
{
    std::ifstream input{ "saves/data.json", std::ios::in };
    Json::Value root;

    input >> root;
    input.close();

    LoadGameEvent lgEvent;
    lgEvent.root = &root;

    notify(&lgEvent);
}
