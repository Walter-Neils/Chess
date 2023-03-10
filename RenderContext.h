//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_RENDERCONTEXT_H
#define CHESS_RENDERCONTEXT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "IRenderable.h"
#include "IBehaviour.h"
#include "InputHandler.h"

class IRenderable;

class RenderContext
{
    sf::RenderWindow* window;
    std::vector<IRenderable*>                 renderables;
    std::vector<IBehaviour*>                  behaviours;
    std::map<std::string, std::vector<void*>> items;
    std::map<std::string, sf::Font>           fonts;
    std::map<std::string, sf::Texture>        textures;
    InputHandler* inputHandler;
public:
    explicit RenderContext(sf::RenderWindow* window);

    [[nodiscard]] sf::RenderWindow* getWindow() const;

    void addRenderable(IRenderable* renderable);

    void addBehaviour(IBehaviour* behaviour);

    template<typename T>
    void addItem(T* item);

    template<typename T>
    std::vector<T*> getItemsOfType();

    void addFont(const std::string& name, const std::string& path);

    [[nodiscard]] sf::Font& getFont(const std::string& name);

    void addTexture(const std::string& name, const std::string& path);

    [[nodiscard]] sf::Texture& getTexture(const std::string& name);

    void render();

    void update();

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    void setInputHandler(InputHandler* inputHandler);

    [[nodiscard]] InputHandler* getInputHandler() const;
};

template<typename T>
inline void RenderContext::addItem(T* item)
{
    if (dynamic_cast<IRenderable*>(item) != nullptr) {
        addRenderable(dynamic_cast<IRenderable*>(item));
    }
    if (dynamic_cast<IBehaviour*>(item) != nullptr) {
        addBehaviour(dynamic_cast<IBehaviour*>(item));
    }
}

template<typename T>
inline std::vector<T*> RenderContext::getItemsOfType()
{
    std::vector<T*> itemsOfType;
    for (auto       item: items[typeid(T).name()]) {
        itemsOfType.push_back(reinterpret_cast<T*>(item));
    }
    return itemsOfType;
}


#endif //CHESS_RENDERCONTEXT_H
