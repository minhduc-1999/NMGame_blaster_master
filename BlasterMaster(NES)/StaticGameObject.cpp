#include "StaticGameObject.h"

CStaticGameObject::CStaticGameObject(float x, float y): CGameObject(x, y)
{
	width = TILE_WIDTH;
	height = TILE_HEIGHT;
}

void CStaticGameObject::AddSprite(LPSPRITE src)
{
	_tile.push_back(src);
}

void CStaticGameObject::Render()
{
	_tile.at(0)->Draw(x, y, -1, 255);
}
