//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_INPUTHANDLEREVENTSTREAM_H
#define CHESS_INPUTHANDLEREVENTSTREAM_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"

class InputHandler;

class InputHandlerEventStream
{
private:
    InputHandler* inputHandler;
    size_t index;
public:
    explicit InputHandlerEventStream(InputHandler* inputHandler);

    bool hasNext();

    sf::Event current();

    sf::Event next();

    void seek(size_t index);

    size_t currentIndex();

    friend class InputHandler;
};


#endif //CHESS_INPUTHANDLEREVENTSTREAM_H
