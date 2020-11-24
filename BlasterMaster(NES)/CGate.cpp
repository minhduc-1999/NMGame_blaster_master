#include "CGate.h"

CGate::CGate(float x, float y, int sectionId, D3DXVECTOR2 tele) :CStaticGameObject(x, y)
{
    SetSize(32, 32);
	desTelePos = tele;
	nextSectionID = sectionId;
}

void CGate::Render()
{
    _tile.at(0)->Draw(x - 8, y - 8, -1);
    //_tile.at(1)->Draw(x + 8, y - 8, -1);
    _tile.at(2)->Draw(x - 8, y + 8, -1);
    //_tile.at(3)->Draw(x + 8, y + 8, -1);
}
