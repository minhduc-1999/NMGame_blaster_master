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
    void UpdateSwitchSection(D3DXVECTOR2 mainPos);
    int GetWidth();
    int GetHeight();

    D3DXVECTOR2 GetPosition();    
    Rect GetBound();

    ~Camera();

private:
    int             mWidth,
                    mHeight;

    D3DXVECTOR2     mPosition;
};

