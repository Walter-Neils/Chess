//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_CHESSBOARD_H
#define CHESS_CHESSBOARD_H

#include "IRenderable.h"
#include "ChessPieceInstance.h"
#include "Configuration.h"

class ChessPieceInstance;

class ChessBoard : public IRenderable, public Configuration
{
public:
    std::vector<ChessPieceInstance*> pieces;

    ChessBoard(RenderContext* renderState, int zIndex);

    void render() override;

    sf::Vector2f getAbsoluteScreenPosition(sf::Vector2i boardPosition);

    float getSquareSizeX();

    float getSquareSizeY();

    void scalePieceSprite(sf::Sprite& sprite);

    sf::Vector2i getBoardPosition(sf::Vector2f screenPosition);

    sf::Vector2i getBoardPosition(sf::Vector2i screenPosition);

    ChessPieceInstance* getPieceAtPosition(sf::Vector2i position);

    bool isPieceAtPosition(sf::Vector2i position);

    std::vector<ChessPieceInstance*>
    rayCast(sf::Vector2i start, sf::Vector2i direction, int distance, std::function<bool(ChessPieceInstance*)> ignore);

    inline std::vector<ChessPieceInstance*> rayCast(sf::Vector2i start, sf::Vector2i direction, int distance)
    {
        return rayCast(start, direction, distance, [](ChessPieceInstance*) { return false; });
    }

    bool capturePiece(ChessPieceInstance* piece);

    friend class ChessPieceInstance;
};


#endif //CHESS_CHESSBOARD_H
