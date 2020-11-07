#pragma once
#include "StaticGameObject.h"
class CGate :
    public CStaticGameObject
{
public:
    CGate(float x, float y) :CStaticGameObject(x, y) { SetSize(32, 32); }
    virtual void Render() { _tile.at(0)->Draw(x, y, -1);
    _tile.at(1)->Draw(x + 16, y, -1);
    _tile.at(2)->Draw(x, y + 16, -1);
    _tile.at(3)->Draw(x + 16, y + 16, -1);
    }
};

