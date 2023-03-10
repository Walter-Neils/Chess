//
// Created by walterineils on 3/9/23.
//

#include "RenderContext.h"

RenderContext::RenderContext(sf::RenderWindow* window)
{
    this->window = window;
}

sf::RenderWindow* RenderContext::getWindow() const
{
    return window;
}

void RenderContext::addRenderable(IRenderable* renderable)
{
    renderable->setRenderState(this);
    renderables.push_back(renderable);
}

void RenderContext::render()
{
    // Organize renderables by zIndex
    std::sort(renderables.begin(), renderables.end(), [](IRenderable* a, IRenderable* b) {
        return a->getZIndex() < b->getZIndex();
    });
    // Display 'em
    for (auto renderable: renderables) {
        renderable->render();
    }
}

void RenderContext::addFont(const std::string& name, const std::string& path)
{
    sf::Font font;
    if (!font.loadFromFile(path)) {
        throw std::runtime_error("Could not load font from path: " + path);
    }
    fonts[name] = font;
}

sf::Font& RenderContext::getFont(const std::string& name)
{
    if (fonts.find(name) == fonts.end())
        throw std::runtime_error("Could not find font with name: " + name);
    return fonts[name];
}

void RenderContext::addTexture(const std::string& name, const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Could not load texture from path: " + path);
    }
    textures[name] = texture;
}

sf::Texture& RenderContext::getTexture(const std::string& name)
{
    if (textures.find(name) == textures.end())
        throw std::runtime_error("Could not find texture with name: " + name);
    return textures[name];
}

int RenderContext::getWidth() const
{
    return window->getView().getSize().x;
}

int RenderContext::getHeight() const
{
    return window->getView().getSize().y;
}

void RenderContext::setInputHandler(InputHandler* inputHandler)
{
    this->inputHandler = inputHandler;
}

InputHandler* RenderContext::getInputHandler() const
{
    return inputHandler;
}

void RenderContext::addBehaviour(IBehaviour* behaviour)
{
    behaviours.push_back(behaviour);
}

void RenderContext::update()
{
    for (auto behaviour: behaviours) {
        behaviour->update();
    }
}


