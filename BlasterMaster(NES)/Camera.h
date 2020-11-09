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
    void SetPosition(float x, float y, D3DXVECTOR3 mapPos, D3DXVECTOR3 mapDimen);
    void SetPosition(D3DXVECTOR3 pos);

    int GetWidth();
    int GetHeight();

    D3DXVECTOR3 GetPosition();    
    Rect GetBound();

    ~Camera();

private:
    int             mWidth,
                    mHeight;

    D3DXVECTOR3     mPosition;
};

