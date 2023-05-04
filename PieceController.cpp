//
// Created by walterineils on 3/9/23.
//

#include "PieceController.h"
#include "ChessBoard.h"
#include "Logging.h"
#include "AIPlayer.h"
#include <algorithm>

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
    if(runWinChecks(false))
    {
        LOG("GAME OVER BY WIN CHECKS");
        return; // Game is over, don't do anything else.
    }
    if(runWinChecks(true))
    {
        LOG("GAME OVER BY WIN CHECKS");
        return; // Game is over, don't do anything else.
    }
    chessBoard->checkForCheckmate(true);
    chessBoard->checkForCheckmate(false);
    if (chessBoard->winner.has_value()) {
        LOG("Winner: " << chessBoard->winner.value());
        return;
    }
    // Is the mouse even in the window?
    if (!inputHandler->isMouseInWindow()) {
        return; // It's not. All the logic below requires the mouse to be in the window.
    }
    bool consoleDebug = getConfigurationValue<bool>("debug", false);
    if (selectedPiece == nullptr) {
        if (inputHandler->isMouseButtonPressed(sf::Mouse::Left) && !isMousePressed) {
            isMousePressed             = true;
            sf::Vector2i mousePosition = inputHandler->getMousePosition();
            auto         selectedTile  = chessBoard->getBoardPosition(mousePosition);
            auto         piece         = chessBoard->getPieceAtPosition(selectedTile);
            if (piece != nullptr) {
                // Make sure that the piece is of the correct team
                if (piece->getIsWhite() != chessBoard->currentTurn) {
                    return;
                }
                selectedPiece = piece;
                selectedPiece->setRenderValidMoves(true);
                selectedPiece->setIsSelected(true);
                if (consoleDebug) {
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
                {
                    AIPlayer aiPlayer(chessBoard, chessBoard->currentTurn, this);
                    float    rating = aiPlayer.rateMove(selectedPiece, selectedTile, true);
                    if(rating < -1000.0F)
                    {
                        // This move isn't good, don't do it.
                        return;
                    }
                    LOG("Player move rating: " << rating);
                }
                movePiece(selectedPiece, selectedTile);
                selectedPiece->setRenderValidMoves(false);
                selectedPiece->setIsSelected(false);
                selectedPiece = nullptr;
                if (consoleDebug) {
                    LOG("Moved piece to position: " + std::to_string(selectedTile.x) + ", " +
                        std::to_string(selectedTile.y));
                }
            }
            else {
                selectedPiece->setRenderValidMoves(false);
                selectedPiece->setIsSelected(false);
                selectedPiece = nullptr;
                if (consoleDebug) {
                    LOG("De-selected piece");
                }
            }
        }
    }

    if (!inputHandler->isMouseButtonPressed(sf::Mouse::Left)) {
        isMousePressed = false;
    }
}

void PieceController::movePiece(ChessPieceInstance* piece, const sf::Vector2i& position)
{
    auto currentPieceAtPosition = chessBoard->getPieceAtPosition(position);
    if (currentPieceAtPosition != nullptr) {
        chessBoard->capturePiece(currentPieceAtPosition);
    }
    piece->setPosition(position);
    // LOG("Moving piece to position: " << position.x << ", " << position.y);
    chessBoard->nextTurn();
}

bool PieceController::runWinChecks(bool team)
{
    if(chessBoard->winner.has_value()) return true;
    // Get all required pieces
    auto requiredPieces = chessBoard->pieces;
    // Remove any pieces for which piece->isRequiredPiece() returns false
    requiredPieces.erase(std::remove_if(requiredPieces.begin(), requiredPieces.end(),
                                        [&](ChessPieceInstance* piece) { return !piece->isRequiredPiece() || piece->getIsWhite() != team; }),
                         requiredPieces.end());

    auto enemyPieces = chessBoard->pieces;
    enemyPieces.erase(std::remove_if(enemyPieces.begin(), enemyPieces.end(),
                                     [&](ChessPieceInstance* piece) { return piece->getIsWhite() == team; }),
                      enemyPieces.end());



    for (auto piece: requiredPieces) {
        bool      isInDanger = false;
        for (auto enemyPiece: enemyPieces) {
            auto validMoves = enemyPiece->getValidMoves();
            if (std::find(validMoves.begin(), validMoves.end(), piece->getPosition()) != validMoves.end()) {
                isInDanger = true;
                break;
            }
        }
        if(!isInDanger) continue;

        if(isInDanger && chessBoard->currentTurn != team)
        {
            return true;
        }

        auto validMoves = piece->getValidMoves();
        if (validMoves.empty()) {
            // chessBoard->winner
            // Winner is the opposite of the team of the piece
            chessBoard->winner = !piece->getIsWhite();
            LOG("GAME OVER: NO VALID MOVES FOR " << piece->getPieceBehaviour()->getPieceName() << " AT " << piece->getPosition().x << ", "
                                                 << piece->getPosition().y);
            return true;
        }

        // Remove all validMoves that overlap with any of the enemy pieces valid moves
        for (auto enemyPiece: enemyPieces) {
            auto enemyValidMoves = enemyPiece->getValidMoves();
            validMoves.erase(std::remove_if(validMoves.begin(), validMoves.end(),
                                            [&enemyValidMoves](const sf::Vector2i& move) {
                                                return std::find(enemyValidMoves.begin(), enemyValidMoves.end(),
                                                                 move) != enemyValidMoves.end();
                                            }),
                             validMoves.end());
        }

        if(!validMoves.empty()) continue;
        // Check if any friendly pieces can move to intercept
        auto friendlyPieces = chessBoard->pieces;
        friendlyPieces.erase(std::remove_if(friendlyPieces.begin(), friendlyPieces.end(),
                                            [&](ChessPieceInstance* piece) { return piece->getIsWhite() != team; }),
                             friendlyPieces.end());
        bool friendlyIntercept = false;
        for(auto friendlyPiece: friendlyPieces)
        {
            auto friendlyValidMoves = friendlyPiece->getValidMoves();
            AIPlayer aiPlayer(chessBoard, chessBoard->currentTurn, this);
            for(auto friendlyMove: friendlyValidMoves)
            {
                float rating = aiPlayer.rateMove(friendlyPiece, friendlyMove, true);
                if(rating > 1000.0F)
                {
                    friendlyIntercept = true;
                    break;
                }
            }
        }

        if(friendlyIntercept)
        {
            LOG("Saved by friendly intercept");
            continue;
        }





        LOG("GAME OVER: NO SAFE MOVES FOR " << piece->getPieceBehaviour()->getPieceName() << " AT " << piece->getPosition().x << ", "
                                            << piece->getPosition().y);
        chessBoard->winner = !piece->getIsWhite();
        return true;
    }

    return false;


}
