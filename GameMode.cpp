//
// Created by walterineils on 3/9/23.
//

#include "GameMode.h"
#include "Logging.h"

PiecePosition::PiecePosition(std::string pieceType, int x, int y)
{
    this->pieceType = pieceType;
    this->x         = x;
    this->y         = y;
}

PiecePosition::PiecePosition(nlohmann::json json)
{
    this->pieceType = json["pieceType"];
    this->x         = json["x"];
    this->y         = json["y"];
}

const std::string& PiecePosition::getPieceType() const
{
    return pieceType;
}

int PiecePosition::getX() const
{
    return x;
}

int PiecePosition::getY() const
{
    return y;
}

GameMode::GameMode(std::string title, int id, std::vector<PiecePosition> piecePositions)
{
    this->title          = title;
    this->id             = id;
    this->piecePositions = piecePositions;
}

GameMode::GameMode(nlohmann::json json)
{
    this->title = json["title"];
    this->id    = json["id"];
    auto piecePositionsJson = json["piecePositions"];
    for (const auto& piecePosition: piecePositionsJson) {
        this->piecePositions.emplace_back(piecePosition);
    }
    LOG("Deserialized GameMode: " + title << " (" << id << ") "
                                          << " with " + std::to_string(piecePositions.size()) + " piece positions.");
}

const std::string& GameMode::getTitle() const
{
    return title;
}

int GameMode::getId() const
{
    return id;
}

const std::vector<PiecePosition>& GameMode::getPiecePositions() const
{
    return piecePositions;
}

std::vector<ChessPieceInstance*>
GameMode::load(ChessBoard* chessBoard, RenderContext* renderContext, AssetLoader* assetLoader)
{
    std::vector<ChessPieceInstance*> pieceInstances;
    for (const auto& piecePosition: piecePositions) {
        try {
            auto pieceBehaviour = assetLoader->getPieceBehaviour(piecePosition.getPieceType());
            auto pieceInstance  = new ChessPieceInstance(pieceBehaviour, chessBoard, renderContext,
                                                         {piecePosition.getX(), piecePosition.getY()}, false);
            pieceInstances.push_back(pieceInstance);
        }
        catch (...) {}
    }
    for (auto piecePosition: piecePositions) {
        try {
            // Other side of the board
            piecePosition = PiecePosition(piecePosition.getPieceType(), 7 - piecePosition.getX(),
                                          7 - piecePosition.getY());

            auto pieceBehaviour = assetLoader->getPieceBehaviour(piecePosition.getPieceType());
            auto pieceInstance  = new ChessPieceInstance(pieceBehaviour, chessBoard, renderContext,
                                                         {piecePosition.getX(), piecePosition.getY()}, true);
            pieceInstances.push_back(pieceInstance);
        }
        catch (...) {}
    }
    for (auto& instance: pieceInstances) {
        renderContext->addItem(instance);
    }
    LOG("Loaded GameMode " << title);
    LOG("Loaded " << pieceInstances.size() << " pieces");
    LOG("Base position count: " << piecePositions.size());
    return pieceInstances;
}
