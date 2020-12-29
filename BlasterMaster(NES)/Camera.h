#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "Utils.h"
class Camera
{
public:
    Camera(int width, int height);

    //center of camera
    void SetPosition(float x, float y);
    void SetPosition(D3DXVECTOR2 pos);
    void Update(D3DXVECTOR2 mainPos, D3DXVECTOR2 mapPos, D3DXVECTOR2 mapDimen);
    void UpdateSwitchSection(DWORD dt, int nx, D3DXVECTOR2 desPos, D3DXVECTOR2 desMapPos, D3DXVECTOR2 desMapDimen);
    int GetWidth();
    int GetHeight();

    D3DXVECTOR2 GetPosition();    
    Rect GetBound();
    Rect GetLockBound();
    ~Camera();

private:
    int             mWidth,
                    mHeight;
    D3DXVECTOR2     mPosition;
};

