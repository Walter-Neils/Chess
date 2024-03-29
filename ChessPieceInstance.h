//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_CHESSPIECEINSTANCE_H
#define CHESS_CHESSPIECEINSTANCE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "IRenderable.h"
#include "ChessBoard.h"
#include "RenderContext.h"
#include "PieceBehaviour.h"
#include "InputHandler.h"
#include "CorruptionDetector.h"
#include "Configuration.h"

class ChessBoard;

class ChessPieceInstance : public IRenderable, public IBehaviour, public Configuration
{
private:
    PieceBehaviour* pieceBehaviour;
    sf::Sprite      sprite;
    sf::CircleShape spriteBackground;

    std::vector<sf::RectangleShape> validMoveUIElements;

    std::vector<sf::Vector2i> validMoveEndPositions;

    ChessBoard   * chessBoard;
    RenderContext* renderContext;
    sf::Vector2i position;
    bool         isWhite;
    bool         hasBeenMoved     = false;
    bool         renderValidMoves = false;
    bool         isSelected       = false;
public:
    ChessPieceInstance(PieceBehaviour* pieceBehaviour, ChessBoard* chessBoard, RenderContext* renderContext,
                       const sf::Vector2i& position, bool isWhite);

    ~ChessPieceInstance() override;

    void render() override;

    void update() override;

    /*
     * Returns a vector of all valid moves for this piece.
     * The sf::Vector2i instances indicate an absolute position on the board, not a position relative to the piece.
     */
    std::vector<sf::Vector2i> getValidMoves();

    void setRenderValidMoves(bool renderValidMoves);

    bool getRenderValidMoves();

    bool isRequiredPiece() const;

    void setPosition(const sf::Vector2i& position);

    void setIsSelected(bool isSelected);

    [[nodiscard]] bool getIsSelected() const;

    [[nodiscard]] sf::Vector2i getPosition() const;

    [[nodiscard]] bool getIsWhite() const;

    [[nodiscard]] bool getHasBeenMoved() const;

    [[nodiscard]] PieceBehaviour* getPieceBehaviour() const;

    [[nodiscard]] sf::Sprite* getSprite();

    [[nodiscard]] int getValue() const;

    bool canMoveToPosition(sf::Vector2i position);

    friend class ChessBoard;
};


#endif //CHESS_CHESSPIECEINSTANCE_H
