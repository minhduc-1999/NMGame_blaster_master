#ifndef _HPBar_H
#define _HPBar_H
#include "DynamicGameObject.h"

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
	int hp;
	HPBar();
	void Update(DWORD dt, float x, float y, int curHP);
	void Render();
};
#endif

