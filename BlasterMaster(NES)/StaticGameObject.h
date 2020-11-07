#pragma once
#include "Sprites.h"
#include "GameObject.h"

#define TILE_WIDTH		16
#define TILE_HEIGHT		16

class CStaticGameObject :
	public CGameObject
{
protected:
	vector<LPSPRITE> _tile;
public:
	CStaticGameObject(float x, float y);
	void AddSprite(LPSPRITE src);
	virtual void Render();
};
typedef CStaticGameObject* LPSTATICOBJECT;

