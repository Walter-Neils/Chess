#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "IRenderable.h"
#include "RenderContext.h"
#include "ChessBoard.h"
#include "InputHandlerEventStream.h"
#include "AssetLoader.h"
#include "ChessPieceInstance.h"
#include "Logging.h"
#include "PieceController.h"
#include "Configuration.h"

int main()
{
    Configuration::loadConfiguration();
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
    RenderContext    renderState(&window);
    ChessBoard       chessBoard(&renderState, 0);
    InputHandler     inputHandler(&window);
    renderState.setInputHandler(&inputHandler);


    AssetLoader assetLoader(&renderState);
    assetLoader.loadAssets();

    std::vector<ChessPieceInstance*> pieces = assetLoader.getGameMode(3)->load(&chessBoard, &renderState, &assetLoader);

    PieceController pieceController(&renderState, &inputHandler, &chessBoard, pieces);

    renderState.addRenderable(&chessBoard);
    InputHandlerEventStream eventStream(&inputHandler);
    try {
        uint16_t loopIteration = 0;
        while (window.isOpen()) {
            loopIteration++;
            inputHandler.update();
            while (eventStream.hasNext()) {
                sf::Event event = eventStream.next();
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            pieceController.update();

            window.clear();
            if (loopIteration % 1 == 0) {
                renderState.update();
            }
            renderState.render();
            window.display();
        }
    }
    catch (...) {
        Configuration::saveConfiguration();
        throw;
    };

    return 0;
}
