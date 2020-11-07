#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Sprites.h"
class CGameObject
{
protected:
	float x, y;
	float width, height;
public:
	CGameObject(float x, float y);
	void SetSize(float width, float height);
	void SetPosition(float x, float y);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetSize();
	virtual void Render() = 0;
	RECT GetBound();
};
typedef CGameObject* LPGAMEOBJECT;
#endif



