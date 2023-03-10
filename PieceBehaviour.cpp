//
// Created by walterineils on 3/9/23.
//

#include "PieceBehaviour.h"
#include <sstream>

PieceBehaviour::PieceBehaviour(const std::string& pieceName, const std::vector<Move>& moves, int value,
                               const std::string& iconName)
{
    this->pieceName = pieceName;
    this->moves     = moves;
    this->value     = value;
    this->iconName  = iconName;
}

const std::string& PieceBehaviour::getPieceName() const
{
    return pieceName;
}

const std::vector<Move>& PieceBehaviour::getMoves() const
{
    return moves;
}

int PieceBehaviour::getValue() const
{
    return value;
}

const std::string& PieceBehaviour::getIconNameRaw() const
{
    return iconName;
}

PieceBehaviour PieceBehaviour::from_json(const nlohmann::json& j)
{
    PieceBehaviour pieceBehaviour;
    j.at("pieceName").get_to(pieceBehaviour.pieceName);
    j.at("pieceValue").get_to(pieceBehaviour.value);
    j.at("pieceIcon").get_to(pieceBehaviour.iconName);
    auto moves = j.at("moves");
    for (auto& move: moves) {
        Move m;
        move.at("type").get_to(m.type);
        move.at("offsetMode").get_to(m.offsetMode);
        move.at("isFirstMove").get_to(m.isFirstMove);
        move.at("isCapture").get_to(m.isCapture);
        auto offset = move.at("offset");
        offset.at("x").get_to(m.x);
        offset.at("y").get_to(m.y);
        pieceBehaviour.moves.push_back(m);
    }
    return pieceBehaviour;
}

std::string PieceBehaviour::getIconName(bool isWhite, bool isOutlined) const
{
    // Icon names are stored as literal "knight-{color}-{variant}.png"
    // where {color} is either "white" or "black" and {variant} is either "outlined" or "filled"

    std::string color   = isWhite ? "white" : "black";
    std::string variant = isOutlined ? "outlined" : "filled";

    // replace the {color} and {variant} placeholders with the actual values
    std::string iconName = this->iconName;
    iconName.replace(iconName.find("{color}"), 7, color);
    iconName.replace(iconName.find("{variant}"), 9, variant);

    return iconName;
}
