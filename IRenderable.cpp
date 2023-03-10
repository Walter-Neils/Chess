//
// Created by walterineils on 3/9/23.
//

#include "IRenderable.h"

int IRenderable::getZIndex() const
{
    return zIndex;
}

RenderContext* IRenderable::getRenderState() const
{
    return renderState;
}

IRenderable::IRenderable(RenderContext* renderState, int zIndex)
{
    this->renderState = renderState;
    this->zIndex = zIndex;
}

void IRenderable::setRenderState(RenderContext* renderState)
{
    this->renderState = renderState;
}
