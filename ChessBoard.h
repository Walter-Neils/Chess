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
private:
    std::optional<bool> mWinner;
    int mWhiteScore = 0;
    int mBlackScore = 0;
    bool mCurrentTurn = true;
    int round = 0;
public:
    std::vector<ChessPieceInstance*> pieces;
    std::optional<bool>& winner = mWinner; // Cheap way to make a read-only reference
    const int& whiteScore = mWhiteScore;
    const int& blackScore = mBlackScore;
    const bool& currentTurn = mCurrentTurn;
    const int& currentRound = round;

    void nextTurn();

    ChessBoard(RenderContext* renderState, int zIndex);

    void render() override;

    void addScore(bool team, int score);

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

    std::vector<ChessPieceInstance*> getPiecesCapableOfMovingToPosition(sf::Vector2i position);

    void capturePiece(ChessPieceInstance* piece);

    void checkForCheckmate(bool team);

    bool isPositionInDanger(sf::Vector2i position, bool team);

    friend class ChessPieceInstance;
};


#endif //CHESS_CHESSBOARD_H
