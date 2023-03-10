//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_INPUTHANDLER_H
#define CHESS_INPUTHANDLER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include "InputHandlerEventStream.h"

class InputHandlerEventStream;

class InputHandler
{
private:
    sf::RenderWindow* window;

    std::vector<sf::Event> events;

    std::vector<InputHandlerEventStream*> eventStreams;
public:
    explicit InputHandler(sf::RenderWindow* window);

    bool isWindowOpen();

    bool isKeyPressed(sf::Keyboard::Key key);

    bool isMouseButtonPressed(sf::Mouse::Button button);

    sf::Vector2i getMousePosition();

    void update();

    void garbageCollect();

    friend class InputHandlerEventStream;
};


#endif //CHESS_INPUTHANDLER_H
