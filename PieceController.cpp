//
// Created by walterineils on 3/9/23.
//

#include "PieceController.h"
#include "ChessBoard.h"
#include "Logging.h"

PieceController::PieceController(RenderContext* renderContext, InputHandler* inputHandler, ChessBoard* chessBoard,
                                 const std::vector<ChessPieceInstance*>& pieces) : Configuration("PieceController"),
                                                                                   eventStream(
                                                                                           InputHandlerEventStream(
                                                                                                   inputHandler))
{
    this->renderContext = renderContext;
    this->inputHandler  = inputHandler;
    this->chessBoard    = chessBoard;
    this->pieces        = pieces;
    this->selectedPiece = nullptr;
}

void PieceController::update()
{
    bool consoleDebug = getConfigurationValue<bool>("debug", false);
    if (selectedPiece == nullptr) {
        if (inputHandler->isMouseButtonPressed(sf::Mouse::Left) && !isMousePressed) {
            isMousePressed = true;
            sf::Vector2i mousePosition = inputHandler->getMousePosition();
            auto         selectedTile  = chessBoard->getBoardPosition(mousePosition);
            auto         piece         = chessBoard->getPieceAtPosition(selectedTile);
            if (piece != nullptr) {
                selectedPiece = piece;
                selectedPiece->setRenderValidMoves(true);
                if(consoleDebug)
                {
                    LOG("Selected piece at position: " + std::to_string(selectedTile.x) + ", " +
                        std::to_string(selectedTile.y));
                    LOG("Valid moves: " + std::to_string(selectedPiece->getValidMoves().size()));
                }
            }
        }
    }
    else {
        if (inputHandler->isMouseButtonPressed(sf::Mouse::Left) && !isMousePressed) {
            isMousePressed = true;
            sf::Vector2i mousePosition = inputHandler->getMousePosition();
            auto         selectedTile  = chessBoard->getBoardPosition(mousePosition);
            // Check if the selected tile is a valid move
            auto         validMoves    = selectedPiece->getValidMoves();
            if (std::find(validMoves.begin(), validMoves.end(), selectedTile) != validMoves.end()) {
                // Move the piece
                selectedPiece->setPosition(selectedTile);
                selectedPiece->setRenderValidMoves(false);
                selectedPiece = nullptr;
                if(consoleDebug)
                {
                    LOG("Moved piece to position: " + std::to_string(selectedTile.x) + ", " +
                        std::to_string(selectedTile.y));
                }
            }
            else {
                selectedPiece->setRenderValidMoves(false);
                selectedPiece = nullptr;
                if(consoleDebug)
                {
                    LOG("De-selected piece");
                }
            }
        }
    }

    if (!inputHandler->isMouseButtonPressed(sf::Mouse::Left)) {
        isMousePressed = false;
    }
}
