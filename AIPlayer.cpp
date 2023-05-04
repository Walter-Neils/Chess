//
// Created by walterineils on 5/3/23.
//

#include "AIPlayer.h"
#include <thread>
#include "Logging.h"
#include "Cleanup.h"

AIPlayer::AIPlayer(ChessBoard* board, bool team, PieceController* pieceController)
{
    this->board           = board;
    this->team            = team;
    this->pieceController = pieceController;
}

void AIPlayer::update()
{
    if (board->currentTurn != team)
        return;

    auto                      pieces = board->pieces;
    // Remove pieces that are not of the team we are looking for
    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [&](ChessPieceInstance* piece) {
        return piece->getIsWhite() != team;
    }), pieces.end());

    struct PossibleMove
    {
        ChessPieceInstance* piece;
        sf::Vector2i move;
        float        value;
    };
//    PossibleMove bestMove{nullptr, {0, 0}, -99999999.0};
    std::vector<PossibleMove> consideredMoves;

    if constexpr (true) {
        for (auto piece: pieces) {
            for (auto move: piece->getValidMoves()) {
                float value = rateMove(piece, move);
                // Push the move to the list of possible moves
                consideredMoves.push_back({piece, move, value});
            }
        }

    }
    else {
        // Offensive moves
        for (auto piece: pieces) {
            auto      possibleMoves = piece->getValidMoves();
            for (auto move: possibleMoves) {
                auto pieceAtTargetPosition = board->getPieceAtPosition(move);
                if (pieceAtTargetPosition != nullptr && pieceAtTargetPosition->getIsWhite() != team) {
                    float value = static_cast<float>(pieceAtTargetPosition->getValue()) /
                                  static_cast<float>(piece->getValue());
                    // Push the move to the list of possible moves
                    consideredMoves.push_back({piece, move, value});
                }
            }
        }

        // Defensive moves
        std::vector<ChessPieceInstance*> enemyPieces;
        {
            auto pieces = board->pieces;
            // Remove pieces that are not of the team we are looking for
            pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [&](ChessPieceInstance* piece) {
                return piece->getIsWhite() == team;
            }), pieces.end());
            enemyPieces = pieces;
        }

        std::vector<ChessPieceInstance*> friendlyPiecesInDanger;
        for (auto                        enemyPiece: enemyPieces) {
            auto      possibleMoves = enemyPiece->getValidMoves();
            for (auto move: possibleMoves) {
                auto pieceAtTargetPosition = board->getPieceAtPosition(move);
                if (pieceAtTargetPosition != nullptr && pieceAtTargetPosition->getIsWhite() == team) {
                    friendlyPiecesInDanger.push_back(pieceAtTargetPosition);
                }
            }
        }

        // Remove duplicates
        std::sort(friendlyPiecesInDanger.begin(), friendlyPiecesInDanger.end());
        friendlyPiecesInDanger.erase(std::unique(friendlyPiecesInDanger.begin(), friendlyPiecesInDanger.end()),
                                     friendlyPiecesInDanger.end());

        for (auto piece: friendlyPiecesInDanger) {
            auto possibleMoves = piece->getValidMoves();

            // Sort moves by y direction, prioritizing moves that move the piece away from the enemy
            std::sort(possibleMoves.begin(), possibleMoves.end(), [&](sf::Vector2i a, sf::Vector2i b) {
                return team ? a.y < b.y : a.y > b.y;
            });

            for (auto move: possibleMoves) {
                float value = static_cast<float>(piece->getValue());
                if (piece->isRequiredPiece()) {
                    value = std::numeric_limits<float>::max(); // This is a really important piece, don't lose it
                }
                auto pieceAtTargetPosition = board->getPieceAtPosition(move);
                // If we are moving to a position that is not empty, we are capturing a piece
                // So we add the value of the piece we are capturing to the value of the move
                if (pieceAtTargetPosition != nullptr) {
                    value += static_cast<float>(pieceAtTargetPosition->getValue());
                }
                // Push the move to the list of possible moves
                consideredMoves.push_back({piece, move, value});
            }
        }
    }

    if (!consideredMoves.empty()) {
        // Sort moves by value
        std::sort(consideredMoves.begin(), consideredMoves.end(), [&](PossibleMove a, PossibleMove b) {
            return a.value > b.value;
        });
        if constexpr(false)
        {
            // Get the best move's value
            float bestMoveValue       = consideredMoves[0].value;
            LOG("Best move value: " << bestMoveValue);
            if(bestMoveValue > 500.0F && consideredMoves[0].piece->isRequiredPiece())
            {
                LOG("Moving required piece");
                pieceController->movePiece(consideredMoves[0].piece, consideredMoves[0].move);
            }
            else
            {
                int   validSelectionRange = std::max(1, static_cast<int>(board->currentRound / 10));
                // Pick a random move
                auto targetIndex = static_cast<unsigned long>(rand()) % std::min(static_cast<unsigned long>(validSelectionRange), static_cast<unsigned long>(consideredMoves.size()));
                targetIndex = std::min(targetIndex, consideredMoves.size() - 1);
                auto move = consideredMoves[targetIndex];
                // Move the piece
                pieceController->movePiece(move.piece, move.move);
                return;
            }
        }
        else
        {
            float bestMoveValue       = consideredMoves[0].value;
            // remove all moves that have a value lower than the best move's value
            consideredMoves.erase(std::remove_if(consideredMoves.begin(), consideredMoves.end(), [&](PossibleMove move) {
                return move.value < bestMoveValue;
            }), consideredMoves.end());

            // Pick a random move
            auto targetIndex = static_cast<unsigned long>(rand()) % consideredMoves.size();

            auto move = consideredMoves[targetIndex];

            // Move the piece
            pieceController->movePiece(move.piece, move.move);
            return;
        }
    }



    // If we get here, we have no moves that can capture a piece
    // So we just move a random piece
    for (int i = 0; i < 500; i++) {
        auto piece         = pieces[static_cast<unsigned long>(rand()) % pieces.size()];
        auto possibleMoves = piece->getValidMoves();
        if (possibleMoves.size() == 0)
            continue;
        auto move = possibleMoves[static_cast<unsigned long>(rand()) % possibleMoves.size()];
        pieceController->movePiece(piece, move);
        return;
    }
}

float AIPlayer::rateMove(ChessPieceInstance* piece, const sf::Vector2i& move, bool log)
{
    float score = 0.0f;

    if (log) {
        std::cin.bad();
    }


    auto pieceAtTargetPosition = board->getPieceAtPosition(move);
    if (pieceAtTargetPosition != nullptr) {
        score += (static_cast<float>(pieceAtTargetPosition->getValue())) * 1.0F;
        if (pieceAtTargetPosition->isRequiredPiece())
            score += 1000.0f;
    }

    bool      isInDanger = false;
    for (auto enemyPiece: board->pieces) {
        if (enemyPiece->getIsWhite() == team)
            continue;
        auto      possibleMoves = enemyPiece->getValidMoves();
        for (auto possibleMove: possibleMoves) {
            if (possibleMove == piece->getPosition()) {
                isInDanger = true;
                break;
            }
        }
        if (isInDanger)
            break;
    }

    bool      wouldMoveIntoDanger = false;
    for (auto enemyPiece: board->pieces) {
        if (enemyPiece->getIsWhite() == team)
            continue;
        auto      possibleMoves = enemyPiece->getValidMoves();
        for (auto possibleMove: possibleMoves) {
            if (possibleMove == move) {
                wouldMoveIntoDanger = true;
                if (log) {
                    auto pieceCausingDanger = enemyPiece->getPieceBehaviour()->getPieceName();
                    LOG("Danger from " << pieceCausingDanger << " at " << enemyPiece->getPosition().x << ", "
                                       << enemyPiece->getPosition().y << " to " << move.x << ", " << move.y << "");
                }
                break;
            }
        }
        if (wouldMoveIntoDanger && !log)
            break;
    }

    if (wouldMoveIntoDanger && !isInDanger) {
        score -= static_cast<float>(piece->getValue());
        if (piece->isRequiredPiece())
            score -= 1000.0f;
    }

    if (!wouldMoveIntoDanger && isInDanger) {
        score += static_cast<float>(piece->getValue());
        if (piece->isRequiredPiece())
        {
            return std::numeric_limits<float>::max(); // Required piece, move must occur
        }
    }

    if constexpr (true) {
        float initialTeamDangerScore = calculateTeamDangerScore();

        auto piecePosition = piece->getPosition();

        decltype(std::find(board->pieces.begin(), board->pieces.end(), pieceAtTargetPosition)) targetIndex;

        if (pieceAtTargetPosition != nullptr) {

            // Temporarily remove the target piece from the board
            targetIndex = std::find(board->pieces.begin(), board->pieces.end(), pieceAtTargetPosition);
            board->pieces.erase(targetIndex);
        }

        // Temporarily move the piece to the target position
        piece->setPosition(move);

        float finalTeamDangerScore = calculateTeamDangerScore();
        if (pieceAtTargetPosition != nullptr) {
            // Add the target piece back to the board (insert into it's original position)
            board->pieces.insert(targetIndex, pieceAtTargetPosition);
        }

        // Move the piece back to its original position
        piece->setPosition(piecePosition);
        float scoreDiff = (initialTeamDangerScore - finalTeamDangerScore);
        if (log) {
            LOG("Score diff: " << scoreDiff << " for piece at " << piece->getPosition().x << ", "
                               << piece->getPosition().y
                               << " moving to " << move.x << ", " << move.y << "");
        }
        score += scoreDiff;
    }

    return score;
}

std::vector<ChessPieceInstance*> AIPlayer::getPiecesInDanger()
{
    std::vector<ChessPieceInstance*> piecesInDanger;
    for (auto                        piece: board->pieces) {
        if (piece->getIsWhite() == team)
            continue;
        auto      possibleMoves = piece->getValidMoves();
        for (auto move: possibleMoves) {
            auto pieceAtTargetPosition = board->getPieceAtPosition(move);
            if (pieceAtTargetPosition != nullptr && pieceAtTargetPosition->getIsWhite() == team) {
                piecesInDanger.push_back(pieceAtTargetPosition);
            }
        }
    }
    // Remove duplicates
    std::sort(piecesInDanger.begin(), piecesInDanger.end());
    piecesInDanger.erase(std::unique(piecesInDanger.begin(), piecesInDanger.end()), piecesInDanger.end());
    return piecesInDanger;
}

float AIPlayer::calculateTeamDangerScore()
{
    float score = 0.0f;

    for (auto piece: getPiecesInDanger()) {
        score += static_cast<float>(piece->getValue());
        if (piece->isRequiredPiece())
        {
            return std::numeric_limits<float>::max(); // Required piece, move must occur
        }
    }

    return score;
}
