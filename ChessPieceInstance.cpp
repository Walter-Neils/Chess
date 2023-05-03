//
// Created by walterineils on 3/9/23.
//

#include "ChessPieceInstance.h"
#include <algorithm>
#include "Logging.h"

ChessPieceInstance::ChessPieceInstance(PieceBehaviour* pieceBehaviour, ChessBoard* chessBoard,
                                       RenderContext* renderContext, const sf::Vector2i& position, bool isWhite)
        : IRenderable(renderContext, 1), Configuration("ChessPieceInstance")
{
    this->pieceBehaviour = pieceBehaviour;
    this->chessBoard     = chessBoard;
    this->renderContext  = renderContext;
    this->position       = position;
    this->isWhite        = isWhite;

    if (chessBoard == nullptr) {
        throw std::runtime_error("ChessBoard is null");
    }

    chessBoard->pieces.push_back(this);

    auto textureName = pieceBehaviour->getIconName(isWhite);

    sprite.setTexture(renderContext->getTexture(textureName));
    sprite.setPosition(chessBoard->getAbsoluteScreenPosition(position));
    chessBoard->scalePieceSprite(sprite);
    this->spriteBackground.setRadius(sprite.getGlobalBounds().width / 2);

    if (isWhite) {
        this->spriteBackground.setFillColor(sf::Color(0, 0, 255, 150));
    }
    else {
        this->spriteBackground.setFillColor(sf::Color(0, 255, 0, 150));
    }
}

void ChessPieceInstance::render()
{
    spriteBackground.setPosition(sprite.getPosition());
    //renderContext->getWindow()->draw(spriteBackground);
    renderContext->getWindow()->draw(sprite);

    if (renderValidMoves) {
        for (auto& validMove: validMoveUIElements) {
            renderContext->getWindow()->draw(validMove);
        }
    }
}

void ChessPieceInstance::update()
{
    this->validMoveUIElements.clear();
    this->validMoveEndPositions.clear();
    if (getConfigurationValue<bool>("movechecks.update.unprompted", false)) {
        auto validMoves = getValidMoves();
        this->validMoveEndPositions = validMoves;
        for (auto& move: validMoves) {
            sf::RectangleShape rect;
            rect.setSize({chessBoard->getSquareSizeX(), chessBoard->getSquareSizeY()});
            rect.setPosition(chessBoard->getAbsoluteScreenPosition(move));
            rect.setFillColor(sf::Color(0, 0, 0, 0));
            // is this a capture move?
            if (chessBoard->isPieceAtPosition(move)) {
                rect.setOutlineColor(sf::Color(255, 0, 0, 255));
            }
            else {
                rect.setOutlineColor(sf::Color(0, 255, 0, 255));
            }
            rect.setOutlineThickness(4);
            this->validMoveUIElements.push_back(rect);
        }
    }
}

std::vector<sf::Vector2i> ChessPieceInstance::getValidMoves()
{
    bool debugMoveRemoval = getConfigurationValue<bool>("debug.moveRemoval", false);
    auto moveBase         = pieceBehaviour->getMoves();

    if (isWhite) {
        // Invert all y values
        for (auto& move: moveBase) {
            move.y *= -1;
        }
    }

    if (debugMoveRemoval) {
        if (isWhite) {
            LOG("Inverted all y values");
        }
        else {
            LOG("Did not invert y values");
        }
    }


    if (debugMoveRemoval) {
        LOG("Starting with " << moveBase.size() << " moves");
    }


    if (hasBeenMoved) {
        // Filter out moves that require the piece to be on it's first move
        moveBase.erase(std::remove_if(moveBase.begin(), moveBase.end(), [&](Move move) {
            if (move.isFirstMove) {
                if (debugMoveRemoval) {
                    LOG("Removing move '" << move.x << ", " << move.y
                                          << "' because it requires the piece to be on it's first move");
                }
            }
            return move.isFirstMove;
        }), moveBase.end());
    }

    std::vector<sf::Vector2i> validMoves;
    // Offset mode 'absolute' is used for moves that are relative to the piece's position but specify an absolute travel distance
    // Offset mode 'multiple' is used for moves that are relative to the piece's position and specify that the piece can move an unlimited amount of times in that direction
    // Type 'slide' is used for moves that are blocked by other pieces
    // Type 'jump' is used for moves that are not blocked by other pieces
    // 'isCapture' specifies whether a move can only be made if it captures another piece (i.e. a pawn's capture move)


    for (auto& move: moveBase) {
        if (move.offsetMode == "absolute") {
            if (move.type == "slide") {
                auto raycastStart = position;
                auto hits         = chessBoard->rayCast(raycastStart, {move.x, move.y},
                                                        std::max(std::abs(move.x), std::abs(move.y)),
                                                        [&](ChessPieceInstance* piece) {
                                                            // Ignore ourselves
                                                            if (piece == this) {
                                                                return true;
                                                            }
                                                            else {
                                                                return false;
                                                            }
                                                        });


                if (getConfigurationValue<bool>("movement.forceEnableCaptureMoves", false)) {
                    validMoves.emplace_back(position.x + move.x, position.y + move.y);
                    if (debugMoveRemoval) {
                        LOG("Adding move '" << move.x << ", " << move.y
                                            << "' because the movement.forceEnableCaptureMoves option is enabled");
                    }
                    continue;
                }

                if (move.isCapture) {
                    if (!hits.empty()) {
                        auto pieceAtPosition = chessBoard->getPieceAtPosition(
                                {position.x + move.x, position.y + move.y});
                        if (pieceAtPosition != nullptr) {
                            if (pieceAtPosition->getIsWhite() != isWhite) {
                                validMoves.emplace_back(position.x + move.x, position.y + move.y);
                                if (debugMoveRemoval) {
                                    LOG("Adding move '" << move.x << ", " << move.y
                                                        << "' because it is a capture move and the piece to capture is on the opposite team");
                                }
                            }
                            else {
                                if (debugMoveRemoval) {
                                    LOG("Removing move '" << move.x << ", " << move.y
                                                          << "' because it is a capture move but the piece to capture is on the same team");
                                }
                            }
                        }
                        else {
                            if (debugMoveRemoval) {
                                LOG("Removing move '" << move.x << ", " << move.y
                                                      << "' because it is a capture move but there is no piece to capture (raycast hit something, but pieceAtPosition is null (what?))");
                            }
                            // throw std::runtime_error("Capture move is not capturing a piece");
                            // This happens when a multi-unit slide move encounters a piece in it's way but the piece is not at the end of the slide
                        }
                    }
                    else {
                        if (debugMoveRemoval) {
                            LOG("Removing move '" << move.x << ", " << move.y
                                                  << "' because it is a capture move but there is no piece to capture (raycast hits empty)");
                        }
                    }
                }
                else {
                    if (hits.empty()) {
                        validMoves.emplace_back(position.x + move.x, position.y + move.y);
                        if (debugMoveRemoval) {
                            LOG("Adding move '" << move.x << ", " << move.y
                                                << "' because it is not a capture move and there is no piece to capture");
                        }
                    }
                    else {
                        bool differentTeams = hits[0]->getIsWhite() != isWhite;
                        if (differentTeams) {
                            validMoves.emplace_back(position.x + move.x, position.y + move.y);
                            if (debugMoveRemoval) {
                                LOG("Adding move '" << move.x << ", " << move.y
                                                    << "' because it is not a capture move but the piece to capture is on the opposite team");
                            }
                        }
                        else {
                            if (debugMoveRemoval) {
                                LOG("Removing move '" << move.x << ", " << move.y
                                                      << "' because it is not a capture move but the piece to capture is on the same team");
                                // Display piece teams
                                LOG("Piece @ (" << hits[0]->getPosition().x << ", " << hits[0]->getPosition().y
                                                << ") is on team " << (hits[0]->getIsWhite() ? "white" : "black"));
                                LOG("Piece @ (" << getPosition().x << ", " << getPosition().y << ") is on team "
                                                << (getIsWhite() ? "white" : "black"));
                            }
                        }
                    }
                }
            }
            else if (move.type == "jump") {
                auto pieceAtPosition = chessBoard->getPieceAtPosition({position.x + move.x, position.y + move.y});
                if (pieceAtPosition == nullptr) {
                    validMoves.emplace_back(position.x + move.x, position.y + move.y);
                }
                else {
                    if (pieceAtPosition->getIsWhite() != isWhite) {
                        validMoves.emplace_back(position.x + move.x, position.y + move.y);
                    }
                }
            }
        }
        else {
            if (move.type == "slide") {
                auto hits            = chessBoard->rayCast(position, {move.x, move.y}, 64);
                auto currentPosition = position;
                bool canMove         = true;
                while (canMove) {
                    currentPosition.x += move.x;
                    currentPosition.y += move.y;
                    if (currentPosition.x < 0 || currentPosition.x > 7 || currentPosition.y < 0 ||
                        currentPosition.y > 7) {
                        break;
                    }
                    for (auto hit: hits) {
                        if (hit->getPosition() == currentPosition) {
                            if (hit->getIsWhite() != isWhite) {
                                validMoves.emplace_back(currentPosition);
                            }
                            canMove = false;
                            break;
                        }
                    }
                    if (canMove) {
                        validMoves.emplace_back(currentPosition);
                    }
                }
            }
        }
    }

    this->validMoveEndPositions = validMoves;
    return validMoves;
}

void ChessPieceInstance::setRenderValidMoves(bool renderValidMoves)
{
    this->renderValidMoves = renderValidMoves;
}

bool ChessPieceInstance::getRenderValidMoves()
{
    return renderValidMoves;
}

void ChessPieceInstance::setPosition(const sf::Vector2i& position)
{
    this->position = position;
    sprite.setPosition(chessBoard->getAbsoluteScreenPosition(position));
    this->hasBeenMoved = true;
}

sf::Vector2i ChessPieceInstance::getPosition() const
{
    return position;
}

bool ChessPieceInstance::getIsWhite() const
{
    return isWhite;
}

bool ChessPieceInstance::getHasBeenMoved() const
{
    return hasBeenMoved;
}

PieceBehaviour* ChessPieceInstance::getPieceBehaviour() const
{
    return pieceBehaviour;
}

sf::Sprite* ChessPieceInstance::getSprite()
{
    return &sprite;
}

void ChessPieceInstance::setIsSelected(bool newValue)
{
    this->isSelected = newValue;
}

bool ChessPieceInstance::getIsSelected() const
{
    return isSelected;
}

ChessPieceInstance::~ChessPieceInstance()
{
    renderContext->removeRenderable(this);
    renderContext->removeBehaviour(this);
    LOG("ChessPieceInstance destroyed");
}
