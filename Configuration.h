//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_CONFIGURATION_H
#define CHESS_CONFIGURATION_H

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>


class Configuration
{
private:
    static nlohmann::json config;
private:
    std::vector<std::string> baseScope;

    std::vector<std::string> requestPath(const std::string& input);

    static void sanitizePath(std::vector<std::string>& path);

    static nlohmann::json& getPathContainer(const std::vector<std::string>& path);

public:
    explicit Configuration(const std::string& baseScope);

    template<typename T>
    inline T getConfigurationValue(const std::string& path, T defaultValue)
    {
        std::vector<std::string> fullPath = requestPath(path);
        sanitizePath(fullPath);
        nlohmann::json& container = getPathContainer(fullPath);
        if (container.is_null()) {
            container = defaultValue;
            return defaultValue;
        }
        return container.get<T>();
    }

    template<typename T>
    inline void setConfigurationValue(const std::string& path, T value)
    {
        std::vector<std::string> fullPath = requestPath(path);
        sanitizePath(fullPath);
        nlohmann::json& container = getPathContainer(fullPath);
        container                 = value;
    }

    static void saveConfiguration();
    static void loadConfiguration();
};


#endif //CHESS_CONFIGURATION_H
