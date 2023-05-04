//
// Created by walterineils on 3/9/23.
//

#include "AssetLoader.h"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

AssetLoader::AssetLoader(RenderContext* renderContext)
{
    this->renderContext = renderContext;
}

void AssetLoader::loadAssets()
{
    loadIcons();
    loadPieceBehaviours();
    loadGameModes();
}

void AssetLoader::loadIcons()
{
    // Textures will be in the './assets/icons' directory
    std::string path = "./assets/icons";

    // Iterate through the directory
    for (const auto& entry: std::filesystem::directory_iterator(path)) {
        // Get the path to the file
        std::string filePath = entry.path();
        // Name is the last part of the path without the extension
        std::string name     = entry.path().filename().replace_extension().string();

        renderContext->addTexture(name, filePath);

        std::cout << "Loaded texture: " << name << " from " << filePath << std::endl;
    }
}

void AssetLoader::loadPieceBehaviours()
{
    // Piece behaviours will be in the './assets/piecedefs' directory as JSON files
    std::string path = "./assets/piecedefs";

    std::vector<PieceBehaviour> pieceBehaviours;

    // Iterate through the directory
    for (const auto& entry: std::filesystem::directory_iterator(path)) {
        std::string    filePath = entry.path();
        nlohmann::json j;

        std::fstream file(filePath, std::ios::in);

        file >> j;

        PieceBehaviour pieceBehaviour = PieceBehaviour::from_json(j);

        std::cout << "Loaded piece behaviour: " << pieceBehaviour.getPieceName() << " from " << filePath << std::endl;

        pieceBehaviours.push_back(pieceBehaviour);
    }

    this->pieceBehaviours = pieceBehaviours;
}

const std::vector<PieceBehaviour>& AssetLoader::getPieceBehaviours() const
{
    return pieceBehaviours;
}

PieceBehaviour* AssetLoader::getPieceBehaviour(const std::string& pieceName)
{
    for (PieceBehaviour& pieceBehaviour: pieceBehaviours) {
        if (pieceBehaviour.getPieceName() == pieceName) {
            return &pieceBehaviour;
        }
    }

    throw std::runtime_error("Could not find piece behaviour for piece name: " + pieceName);
}

void AssetLoader::loadGameModes()
{
    // Gamemodes will be in the './assets/piecedefs' directory as JSON files
    std::string path = "./assets/gamemodes";

    std::vector<GameMode> gameModes;

    // Iterate through the directory
    for (const auto& entry: std::filesystem::directory_iterator(path)) {
        std::string    filePath = entry.path();
        nlohmann::json j;

        std::fstream file(filePath, std::ios::in);

        file >> j;

        auto gameMode = GameMode(j);

        std::cout << "Loaded game mode: " << gameMode.getTitle() << " from " << filePath << std::endl;

        gameModes.push_back(gameMode);
    }

    this->gameModes = gameModes;
}

const std::vector<GameMode>& AssetLoader::getGameModes() const
{
    return gameModes;
}

GameMode* AssetLoader::getGameMode(int id)
{
    for (GameMode& gameMode: gameModes) {
        if (gameMode.getId() == id) {
            return &gameMode;
        }
    }

    throw std::runtime_error("Could not find game mode for id: " + std::to_string(id));
}

sf::Font* AssetLoader::getFont(const std::string& fontName)
{
    if (fonts.find(fontName) == fonts.end()) {
        sf::Font font;
        font.loadFromFile("./assets/fonts/" + fontName + ".ttf");
        fonts[fontName] = font;
    }

    return &fonts[fontName];
}
