#include "BaseBullet.h"

BaseBullet::BaseBullet(float x, float y, int team, int nx, int ny) : CDynamicGameObject(x, y)
{
	startFiringTime = GetTickCount64();
	this->team = team;
}
