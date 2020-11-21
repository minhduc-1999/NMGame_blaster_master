#include "Camera.h"


Camera::Camera(int width, int height)
{
    mWidth = width ;
    mHeight = height ;

    mPosition = D3DXVECTOR2(0, 0);
}


Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y)
{
    SetPosition(D3DXVECTOR2(x, y));
}

void Camera::SetPosition(float x, float y, D3DXVECTOR2 mapPos, D3DXVECTOR2 mapDimen)
{
    int lastX = x, lastY = y;
    if (x < mapPos.x)
    {
        lastX = mapPos.x;
    }
    else
    {
        if (x > mapPos.x + mapDimen.x - mWidth)
            lastX = mapPos.x + mapDimen.x - mWidth;
    }
    if (y < mapPos.y)
    {
        lastY = mapPos.y;
    }
    else
    {
        if (y > mapPos.y + mapDimen.y - mHeight)
            lastY = mapPos.y + mapDimen.y - mHeight;
    }
    this->SetPosition(lastX, lastY);
}

void Camera::SetPosition(D3DXVECTOR2 pos)
{
    mPosition = pos;
}

D3DXVECTOR2 Camera::GetPosition()
{
    return mPosition;
}

Rect Camera::GetBound()
{
    Rect bound;

    bound.left = mPosition.x;
    bound.right = bound.left + mWidth - 1;
    bound.top = mPosition.y;
    bound.bottom = bound.top + mHeight - 1;

    return bound;
}

int Camera::GetWidth()
{
    return mWidth;
}

int Camera::GetHeight()
{
    return mHeight;
}