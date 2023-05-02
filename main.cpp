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
    bool restart = false;
    do {
        restart = false;
        Configuration::loadConfiguration();
        sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
        RenderContext    renderState(&window);
        ChessBoard       chessBoard(&renderState, 0);
        InputHandler     inputHandler(&window);
        renderState.setInputHandler(&inputHandler);


        AssetLoader assetLoader(&renderState);
        assetLoader.loadAssets();

        std::vector<ChessPieceInstance*> pieces = assetLoader.getGameMode(4)->load(&chessBoard, &renderState,
                                                                                   &assetLoader);

        PieceController pieceController(&renderState, &inputHandler, &chessBoard, pieces);

        renderState.addRenderable(&chessBoard);
        InputHandlerEventStream eventStream(&inputHandler);
        while (window.isOpen()) {
            inputHandler.update();

            while (eventStream.hasNext()) {
                sf::Event event = eventStream.next();
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // If the user presses the escape key, close the window
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    restart = true;
                }
            }

            pieceController.update();

            window.clear();
            renderState.update();
            renderState.render();
            window.display();
        }

        Configuration::saveConfiguration();
    } while (restart);
    return 0;
}
