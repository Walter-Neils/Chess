//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_ASSETLOADER_H
#define CHESS_ASSETLOADER_H

#include "RenderContext.h"
#include "PieceBehaviour.h"
#include "GameMode.h"

class GameMode;

class AssetLoader
{
private:
    RenderContext* renderContext;
    std::vector<PieceBehaviour> pieceBehaviours;
    std::vector<GameMode> gameModes;
public:
    explicit AssetLoader(RenderContext* renderContext);

    void loadAssets();

    [[nodiscard]] const std::vector<PieceBehaviour>& getPieceBehaviours() const;

    [[nodiscard]] PieceBehaviour* getPieceBehaviour(const std::string& pieceName);

    [[nodiscard]] const std::vector<GameMode>& getGameModes() const;

    [[nodiscard]] GameMode* getGameMode(int id);
private:
    void loadPieceBehaviours();
    void loadGameModes();

private:
    void loadIcons();
};


#endif //CHESS_ASSETLOADER_H
