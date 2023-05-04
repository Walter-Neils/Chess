//
// Created by walterineils on 5/3/23.
//

#include "HeatmapWindow.h"
#include <algorithm>

HeatmapWindow::HeatmapWindow(ChessBoard* board, bool team)
{
    this->board = board;
    this->team  = team;
    window.create(sf::VideoMode(800, 800), "Heatmap");
}

void HeatmapWindow::render()
{
    bool lastValue = this->board->currentTurn;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if(lastValue == this->board->currentTurn) {
            continue;
        }
        lastValue = this->board->currentTurn;
        window.clear(sf::Color::White);
        auto     squareSizeX = window.getView().getSize().x / 8;
        auto     squareSizeY = window.getView().getSize().y / 8;
        for (int i           = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto isEnemyPieceAtTargetPosition    = board->getPieceAtPosition({i, j}) != nullptr &&
                                                       board->getPieceAtPosition({i, j})->getIsWhite() != team;
                auto isFriendlyPieceAtTargetPosition = board->getPieceAtPosition({i, j}) != nullptr &&
                                                       board->getPieceAtPosition({i, j})->getIsWhite() == team;
                auto pieces                          = board->getPiecesCapableOfMovingToPosition({i, j});
                // Remove pieces that are not of the team we are looking for
                pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [&](ChessPieceInstance* piece) {
                    return piece->getIsWhite() != team;
                }), pieces.end());
                auto               rgb = 255 - (pieces.size() * 255 / 8);
                sf::RectangleShape square;
                square.setSize(sf::Vector2f(squareSizeX, squareSizeY));
                square.setPosition(i * squareSizeX, j * squareSizeY);
                if (isEnemyPieceAtTargetPosition) {
                    if (rgb != 255) {
                        square.setFillColor(sf::Color::Red);
                    }
                }
                else if (isFriendlyPieceAtTargetPosition) {
                    square.setFillColor(sf::Color::Blue);
                }
                else {
                    square.setFillColor(sf::Color(rgb, rgb, rgb));
                }
                window.draw(square);
            }
        }
        window.display();
    }

}
