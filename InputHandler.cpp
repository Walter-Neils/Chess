//
// Created by walterineils on 3/9/23.
//

#include "InputHandler.h"

InputHandler::InputHandler(sf::RenderWindow* window)
{
    this->window = window;
}

bool InputHandler::isWindowOpen()
{
    return window->isOpen();
}

bool InputHandler::isKeyPressed(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

bool InputHandler::isMouseButtonPressed(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i InputHandler::getMousePosition()
{
    return sf::Mouse::getPosition(*window);
}

void InputHandler::update()
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        events.push_back(event);
    }
}

void InputHandler::garbageCollect()
{
    size_t minIndex = events.size();
    for (auto eventStream : eventStreams)
    {
        if (eventStream->currentIndex() < minIndex)
        {
            minIndex = eventStream->currentIndex();
        }
    }
    if (minIndex > 0)
    {
        events.erase(events.begin(), events.begin() + static_cast<long>(minIndex));
        for (auto eventStream : eventStreams)
        {
            eventStream->seek(eventStream->currentIndex() - minIndex);
        }
    }
}
