#pragma once
#include "CGate.h"

class Panel
{
protected:
	float x, y, vx, vy;
	int width, height;
	bool done;
	unsigned long timeDone;
public:
	Panel(float x, float y, float vx, float vy) 
	{
		this->x = x; this->y = y; this->vx = vx; this->vy = vy;
		width = 0;
		height = 0;
		done = false;
		timeDone = 0;
	}
	virtual void Update(unsigned long dt) = 0;
	virtual void Render() = 0;
	virtual bool IsDone() 
	{
		return done;
	}
	void Finish() { done = true; timeDone = GetTickCount64(); }
};