//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_PIECECONTROLLER_H
#define CHESS_PIECECONTROLLER_H

#include "InputHandlerEventStream.h"
#include "InputHandler.h"
#include "RenderContext.h"
#include "ChessPieceInstance.h"
#include "Configuration.h"


class PieceController : public IBehaviour, public Configuration
{
private:
    std::vector<ChessPieceInstance*> pieces;
    ChessBoard* chessBoard;
    RenderContext* renderContext;
    InputHandler* inputHandler;
    InputHandlerEventStream eventStream;
    ChessPieceInstance* selectedPiece;

    bool isMousePressed = false;
public:
    PieceController(RenderContext* renderContext, InputHandler* inputHandler, ChessBoard* chessBoard, const std::vector<ChessPieceInstance*>& pieces);
    void update() override;

};


#endif //CHESS_PIECECONTROLLER_H
