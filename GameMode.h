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
/*
 * {
    "title": "Standard Game",
    "id": 0,
    "piecePositions": [
        {
            "pieceType": "rook",
            "x": 0,
            "y": 0
        },
        {
            "pieceType": "knight",
            "x": 1,
            "y": 0
        },
        {
            "pieceType": "bishop",
            "x": 2,
            "y": 0
        },
        {
            "pieceType": "queen",
            "x": 3,
            "y": 0
        },
        {
            "pieceType": "king",
            "x": 4,
            "y": 0
        },
        {
            "pieceType": "bishop",
            "x": 5,
            "y": 0
        },
        {
            "pieceType": "knight",
            "x": 6,
            "y": 0
        },
        {
            "pieceType": "rook",
            "x": 7,
            "y": 0
        },
        {
            "pieceType": "pawn",
            "x": 0,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 1,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 2,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 3,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 4,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 5,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 6,
            "y": 1
        },
        {
            "pieceType": "pawn",
            "x": 7,
            "y": 1
        }
    ]
}
 * */

#endif //CHESS_GAMEMODE_H
