#ifndef _HPBar_H
#define _HPBar_H
#include "DynamicGameObject.h"

#define	JASON_MAX_HP				8
#define	SOPHIA_MAX_HP				16
#define SOPHIA_HPBAR				99
#define JASON_HPBAR					100
#define HP_UP						98
#define	HP_DOWN						97

#define HP_BAR_0					8
#define HP_BAR_1					7
#define	HP_BAR_2					6
#define	HP_BAR_3					5
#define	HP_BAR_4					4	
#define	HP_BAR_5					3
#define	HP_BAR_6					2
#define	HP_BAR_7					1	
#define	HP_BAR_FULL					0


class HPBar : public CDynamicGameObject
{
public:
	int hp,jhp,shp,bar;
	bool curBar;
	HPBar();
	void Update(DWORD dt, float x, float y);
	void Render();
	void SetState(int bar);
};
#endif

