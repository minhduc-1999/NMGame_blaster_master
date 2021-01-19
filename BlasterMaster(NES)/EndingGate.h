#pragma once
#include "StaticGameObject.h"
class EndingGate :
    public CStaticGameObject
{
public:
    EndingGate(float x, float y) :CStaticGameObject(x, y) { SetSize(17, 16); }
};