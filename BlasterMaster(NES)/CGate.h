#pragma once
#include "StaticGameObject.h"
class CGate :
    public CStaticGameObject
{
protected:
    D3DXVECTOR2 desTelePos;
    int nextSectionID;
    int nx, ny;
public:
    CGate(float x, float y, int sectionId, D3DXVECTOR2 tele, int width, int height);
    int GetNextSectionID() { return nextSectionID; }
    D3DXVECTOR2 GetDesTelePos() { return desTelePos; }
    virtual void Render();
    Rect GetBound();
    int GetDirectionX() { return nx; }
    int GetDirectionY() { return ny; }
};

