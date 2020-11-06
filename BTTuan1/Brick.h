#pragma once
#include "StaticGameObject.h"
class Brick :
	public StaticGameObject
{
public:
	// Brick() :StaticGameObject() {}
	Brick(float x, float y) :StaticGameObject(x, y) {}
};

