//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_IRENDERABLE_H
#define CHESS_IRENDERABLE_H

#include <SFML/Graphics.hpp>
#include "RenderContext.h"

class RenderContext;

class IRenderable
{
    RenderContext* renderState;
    int zIndex;
private:
    void setRenderState(RenderContext* renderState);
public:
    IRenderable(RenderContext* renderState, int zIndex);

    virtual void render() = 0;

    [[nodiscard]] int getZIndex() const;

    [[nodiscard]] RenderContext* getRenderState() const;

    virtual ~IRenderable() = default;

    friend class RenderContext;
};


#endif //CHESS_IRENDERABLE_H
