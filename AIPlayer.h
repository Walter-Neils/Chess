//
// Created by walterineils on 5/3/23.
//

#ifndef CHESS_AIPLAYER_H
#define CHESS_AIPLAYER_H


#include "ChessBoard.h"
#include "PieceController.h"


class AIPlayer
{
    ChessBoard* board;
    bool team;
    PieceController* pieceController;
public:
    int aggressiveness = 1;
    AIPlayer(ChessBoard* board, bool team, PieceController* pieceController);
    float rateMove(ChessPieceInstance* piece, const sf::Vector2i& move, bool log = false);
    std::vector<ChessPieceInstance*> getPiecesInDanger();
    float calculateTeamDangerScore();
    void update();
};


#endif //CHESS_AIPLAYER_H
