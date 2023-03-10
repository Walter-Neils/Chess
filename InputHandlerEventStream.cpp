//
// Created by walterineils on 3/9/23.
//

#include "InputHandlerEventStream.h"

InputHandlerEventStream::InputHandlerEventStream(InputHandler* inputHandler)
{
    this->inputHandler = inputHandler;
    index = inputHandler->events.size();
}

bool InputHandlerEventStream::hasNext()
{
    return index < inputHandler->events.size();
}

sf::Event InputHandlerEventStream::next()
{
    return inputHandler->events[index++];
}

void InputHandlerEventStream::seek(size_t index)
{
    this->index = index;
}

sf::Event InputHandlerEventStream::current()
{
    return inputHandler->events[index];
}

size_t InputHandlerEventStream::currentIndex()
{
    return index;
}
