//
// Created by walterineils on 3/9/23.
//

#include "ChessBoard.h"

ChessBoard::ChessBoard(RenderContext* renderState, int zIndex) : IRenderable(renderState, zIndex), Configuration("ChessBoard")
{

}

void ChessBoard::render()
{
    auto     window      = getRenderState()->getWindow();
    // Chess board is an 8x8 grid of squares
    auto     squareSizeX = window->getView().getSize().x / 8;
    auto     squareSizeY = window->getView().getSize().y / 8;
    // Draw the squares
    for (int i           = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sf::RectangleShape square;
            square.setSize(sf::Vector2f(squareSizeX, squareSizeY));
            square.setPosition(i * squareSizeX, j * squareSizeY);
            if ((i + j) % 2 == 0) {
                square.setFillColor(sf::Color::White);
            }
            else {
                square.setFillColor(sf::Color::Black);
            }
            window->draw(square);
        }
    }

}

sf::Vector2f ChessBoard::getAbsoluteScreenPosition(sf::Vector2i boardPosition)
{
    auto window      = getRenderState()->getWindow();
    auto squareSizeX = window->getView().getSize().x / 8;
    auto squareSizeY = window->getView().getSize().y / 8;
    return {boardPosition.x * squareSizeX,
            boardPosition.y * squareSizeY}; // NOLINT(cppcoreguidelines-narrowing-conversions)
}

float ChessBoard::getSquareSizeX()
{
    auto window = getRenderState()->getWindow();
    return window->getView().getSize().x / 8;
}

float ChessBoard::getSquareSizeY()
{
    auto window = getRenderState()->getWindow();
    return window->getView().getSize().y / 8;
}

void ChessBoard::scalePieceSprite(sf::Sprite& sprite)
{
    auto squareSizeX = getSquareSizeX();
    auto squareSizeY = getSquareSizeY();
    auto scale       = std::min(squareSizeX / sprite.getGlobalBounds().width,
                                squareSizeY / sprite.getGlobalBounds().height);
    sprite.setScale(scale, scale);
}

ChessPieceInstance* ChessBoard::getPieceAtPosition(sf::Vector2i position)
{
    for (auto piece: pieces) {
        if (piece->position == position) {
            return piece;
        }
    }
    return nullptr;
}

sf::Vector2i ChessBoard::getBoardPosition(sf::Vector2f screenPosition)
{
    auto window      = getRenderState()->getWindow();
    auto squareSizeX = window->getView().getSize().x / 8;
    auto squareSizeY = window->getView().getSize().y / 8;
    return {static_cast<int>(screenPosition.x / squareSizeX), static_cast<int>(screenPosition.y / squareSizeY)};
}

sf::Vector2i ChessBoard::getBoardPosition(sf::Vector2i screenPosition)
{
    auto window      = getRenderState()->getWindow();
    auto squareSizeX = window->getView().getSize().x / 8;
    auto squareSizeY = window->getView().getSize().y / 8;
    sf::Vector2i result = {static_cast<int>(screenPosition.x / squareSizeX), static_cast<int>(screenPosition.y / squareSizeY)};
    if(result.x < 0 || result.x > 7 || result.y < 0 || result.y > 7)
    {
        throw std::out_of_range("Position is out of bounds");
    }
    return result;
}

bool ChessBoard::isPieceAtPosition(sf::Vector2i position)
{
    for (auto piece: pieces) {
        if (piece->position == position) {
            return true;
        }
    }
    return false;
}

std::vector<ChessPieceInstance*> ChessBoard::rayCast(sf::Vector2i start, sf::Vector2i direction, int distance, std::function<bool(ChessPieceInstance*)> ignorePred)
{
    if(distance < 0)
    {
        throw std::invalid_argument("Distance must be greater than 0");
    }

    std::vector<ChessPieceInstance*> pieces;
    bool consoleDebug = getConfigurationValue<bool>("raycast.debug", false);

    if(consoleDebug)
    {
        std::cout << "Raycast @ " << "(" << start.x << ", " << start.y << ") " << "Direction: (" << direction.x << ", " << direction.y << ") Distance: " << distance << " : [";
    }
    for (int                         i = 0; i <= distance; i++) {
        auto position = start + (direction * i);
        auto piece = getPieceAtPosition(position);
        if (piece != nullptr) {
            if(consoleDebug)
            {
                std::cout << "!HIT!";
            }
            pieces.push_back(piece);
        }
        if(consoleDebug)
        {
            std::cout << "(" << position.x << ", " << position.y << "), ";
        }
    }
    if(consoleDebug)
    {
        std::cout << "]" << std::endl;
    }

    // Remove pieces that are ignored
    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), ignorePred), pieces.end());

    return pieces;
}

bool ChessBoard::capturePiece(ChessPieceInstance* piece)
{
    auto it = std::find(pieces.begin(), pieces.end(), piece);
    if (it != pieces.end()) {
        pieces.erase(it);
        delete piece; // Chess piece instance is no longer needed
        return true;
    }
    return false;
}
