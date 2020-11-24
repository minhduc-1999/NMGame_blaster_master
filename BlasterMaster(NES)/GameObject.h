#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Sprites.h"
#include "Utils.h"

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};
class CGameObject
{
protected:
	float x, y;
	float vx, vy;
	float width, height;
public:
	CGameObject(float x, float y);
	void SetSize(float width, float height);
	void SetPosition(float x, float y);
	D3DXVECTOR2 GetPosition();
	D3DXVECTOR2 GetSize();
	void SetSpeed(float vx, float vy);
	D3DXVECTOR2 GetSpeed();
	virtual void Render() = 0;
	virtual Rect GetBound();
};
#endif



