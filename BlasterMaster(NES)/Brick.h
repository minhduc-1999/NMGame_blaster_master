#pragma once
#include "StaticGameObject.h"
class Brick :
	public CStaticGameObject
{
public:
	Brick(float x, float y) :CStaticGameObject(x, y) {}
};

