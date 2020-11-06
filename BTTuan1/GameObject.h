#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Sprites.h"
class CGameObject
{
protected:
	float x, y;
	float vx, vy;
	int nx;
	int state;
	vector<LPANIMATION> animations;
public:
	CGameObject();
	CGameObject(float x, float y);


	void SetPosition(float x, float y) { this->x = x; this->y = y; };
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
	virtual void SetState(int state) { this->state = state; }

	int GetState() { return this->state; }
	float GetX() { return x; };
	float GetY() { return y; };

	void AddAnimation(int aniId);

	virtual void Update(DWORD dt)
	{
		x += vx * dt;
		y += vy * dt;
	}
	virtual void Render() = 0;
};
typedef CGameObject* LPGAMEOBJECT;
#endif // !1



