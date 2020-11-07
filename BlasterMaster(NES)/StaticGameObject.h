#pragma once
#include "Sprites.h"

#define TILE_WIDTH		16
#define TILE_HEIGHT		16

class StaticGameObject
{
protected:
	int x, y, width, height;
	vector<LPSPRITE> _tile;
public:
	//() { width = TILE_WIDTH; height = TILE_HEIGHT; }
	StaticGameObject(float x, float y){ SetPosition(x, y);  width = TILE_WIDTH; height = TILE_HEIGHT;}
	void SetPosition(float x, float y) { this->x = x; this->y = y; };
	void SetSize(float width, float height) { this->width = width; this->height = height; }
	float GetX() { return x; };
	float GetY() { return y; };
	void SetSprite(LPSPRITE src) { _tile.push_back(src); }
	virtual void Render() { _tile.at(0)->Draw(x, y, -1); }
	//~StaticGameObject() {}
};

typedef StaticGameObject* LPSTATICOBJECT;

