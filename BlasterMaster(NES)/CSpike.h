#pragma once
#include "StaticGameObject.h"
class CSpike :
    public CStaticGameObject
{
public:
    CSpike(float x, float y) :CStaticGameObject(x, y) {}
};