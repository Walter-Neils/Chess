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
#include "HeatmapWindow.h"
#include "AIPlayer.h"

int main()
{
    bool restart = false;
    do {
        restart = false;
        srand(time(nullptr));
        Configuration::loadConfiguration();
        sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
        RenderContext    renderState(&window);
        ChessBoard       chessBoard(&renderState, 0);
        InputHandler     inputHandler(&window);
        renderState.setInputHandler(&inputHandler);

        AssetLoader assetLoader(&renderState);
        assetLoader.loadAssets();

        std::vector<ChessPieceInstance*> pieces = assetLoader.getGameMode(0)->load(&chessBoard, &renderState,
                                                                                   &assetLoader);

        bool playerIsWhite = true;

        PieceController pieceController(&renderState, &inputHandler, &chessBoard, pieces);
        renderState.addRenderable(&chessBoard);
        InputHandlerEventStream eventStream(&inputHandler);

        AIPlayer aiPlayer(&chessBoard, true, &pieceController);
        AIPlayer aiPlayer2(&chessBoard, false, &pieceController);

        if constexpr (false)
        {
            std::thread([&] () {
                HeatmapWindow heatmapWindow(&chessBoard, true);
                heatmapWindow.render();
            }).detach();
            std::thread([&] () {
                HeatmapWindow heatmapWindow(&chessBoard, false);
                heatmapWindow.render();
            }).detach();
        }

        int frame = 0;
        while (window.isOpen()) {
            frame++;
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
                    break;
                }
            }

            pieceController.update();

            const int aiUpdateRate = 2;
            if(frame % aiUpdateRate == 0)
            {
                if(!playerIsWhite)
                {
                    aiPlayer.update();
                }
                else
                {
                    aiPlayer2.update();
                }
            }

            window.clear();
            renderState.update();
            renderState.render();
            window.display();

            if(chessBoard.winner.has_value())
            {
                std::cout << "Winner: " << (chessBoard.winner.value() ? "white" : "black") << std::endl;
                while(true)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds (10));
                    // Wait for escape key to be pressed
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
                            break;
                        }
                    }
                }
                break;
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        Configuration::saveConfiguration();
    } while (restart);

    return 0;
}
