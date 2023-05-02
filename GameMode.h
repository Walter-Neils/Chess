//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_GAMEMODE_H
#define CHESS_GAMEMODE_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "ChessBoard.h"
#include "RenderContext.h"
#include "PieceBehaviour.h"
#include "ChessPieceInstance.h"
#include "AssetLoader.h"

class AssetLoader;

class PiecePosition
{
private:
    std::string pieceType;
    int         x;
    int         y;
public:
    PiecePosition(std::string pieceType, int x, int y);

    explicit PiecePosition(nlohmann::json json);

    [[nodiscard]] const std::string& getPieceType() const;

    [[nodiscard]] int getX() const;

    [[nodiscard]] int getY() const;
};


class GameMode
{
private:
    std::string                title;
    int                        id;
    std::vector<PiecePosition> piecePositions;
public:
    GameMode(std::string title, int id, std::vector<PiecePosition> piecePositions);

    explicit GameMode(nlohmann::json json);

    std::vector<ChessPieceInstance*>
    load(ChessBoard* chessBoard, RenderContext* renderContext, AssetLoader* assetLoader);

    [[nodiscard]] const std::string& getTitle() const;

    [[nodiscard]] int getId() const;

    [[nodiscard]] const std::vector<PiecePosition>& getPiecePositions() const;
};
#endif //CHESS_GAMEMODE_H
