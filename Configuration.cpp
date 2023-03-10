//
// Created by walterineils on 3/9/23.
//

#include "Configuration.h"
#include <sstream>
#include <fstream>

nlohmann::json Configuration::config;

std::vector<std::string> Configuration::requestPath(const std::string& input)
{
    std::vector<std::string> results;

    for (const auto& item: baseScope)
        results.push_back(item);

    std::stringstream ss(input);
    std::string       item;
    while (std::getline(ss, item, '.')) {
        results.push_back(item);
    }

    return results;
}

void Configuration::sanitizePath(std::vector<std::string>& path)
{
    // Ensure that the path is valid on the config object. If not, create the path.
    nlohmann::json * current = &config;
    for (const auto& item: path) {
        if (current->find(item) == current->end()) {
            // The item does not exist, so we create it.
            (*current)[item] = nlohmann::json();
        }
        current = &(*current)[item];
    }
}

Configuration::Configuration(const std::string& baseScope)
{
    this->baseScope = requestPath(baseScope);
}

nlohmann::json& Configuration::getPathContainer(const std::vector<std::string>& path)
{
    nlohmann::json * current = &config;
    for (const auto& item: path) {
        current = &(*current)[item];
    }
    return *current;
}

void Configuration::saveConfiguration()
{
    std::ofstream file("config.json");
    file << config.dump(4);
}

void Configuration::loadConfiguration()
{
    std::ifstream file("config.json");
    file >> config;
}
