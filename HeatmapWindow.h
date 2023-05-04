//
// Created by walterineils on 5/3/23.
//

#ifndef CHESS_HEATMAPWINDOW_H
#define CHESS_HEATMAPWINDOW_H

#include "ChessBoard.h"
#include <SFML/Graphics/RenderWindow.hpp>

class HeatmapWindow
{
private:
    sf::RenderWindow window;
    ChessBoard* board;
    bool team;
public:
    HeatmapWindow(ChessBoard* board, bool team);

    void render();
};


#endif //CHESS_HEATMAPWINDOW_H
