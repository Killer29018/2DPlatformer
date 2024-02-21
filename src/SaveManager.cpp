#include "SaveManager.hpp"

#include <fstream>
#include <iostream>

SaveManager::SaveManager() {}

void SaveManager::saveGame()
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "    ";

    Json::Value root;
    // Emit Event

    root["test"]["test2"]["test3"] = 3;

    std::ofstream output{ "saves/data.json", std::ios::in | std::ios::trunc };

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &output);

    output.close();

    std::cout << "Saved\n";
}
