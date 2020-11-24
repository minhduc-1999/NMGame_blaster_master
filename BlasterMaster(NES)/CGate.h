#pragma once
#include "StaticGameObject.h"
class CGate :
    public CStaticGameObject
{
private:
    D3DXVECTOR2 desTelePos;
    int nextSectionID;
    int nx;
public:
    CGate(float x, float y) :CStaticGameObject(x, y) { SetSize(32, 32); this->nx = nx; }
    CGate(float x, float y, int sectionId, D3DXVECTOR2 tele);
    int GetNextSectionID() { return nextSectionID; }
    D3DXVECTOR2 GetDesTelePos() { return desTelePos; }
    virtual void Render();
    
};

