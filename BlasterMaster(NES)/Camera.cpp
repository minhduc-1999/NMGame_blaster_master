#include "Camera.h"


Camera::Camera(int width, int height)
{
	mWidth = width;
	mHeight = height;

	mPosition = D3DXVECTOR2(0, 0);
}


Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y)
{
	SetPosition(D3DXVECTOR2(x, y));
}

void Camera::SetPosition(D3DXVECTOR2 pos)
{
	mPosition = pos;
}

void Camera::Update(D3DXVECTOR2 mainPos, D3DXVECTOR2 mapPos, D3DXVECTOR2 mapDimen)
{
	int lastX = mainPos.x - (mWidth / 2), lastY = mainPos.y - (mHeight / 2);
	if (lastX < mapPos.x)
	{
		lastX = mapPos.x;
	}
	else
	{
		if (lastX > mapPos.x + mapDimen.x - mWidth)
			lastX = mapPos.x + mapDimen.x - mWidth;
	}
	if (lastY < mapPos.y)
	{
		lastY = mapPos.y;
	}
	else
	{
		if (lastY > mapPos.y + mapDimen.y - mHeight)
			lastY = mapPos.y + mapDimen.y - mHeight;
	}
	this->SetPosition(lastX, lastY);
	DebugOut("[UPDATE CAM]\t%f\t%f\n", mPosition.x, mPosition.y);
}

void Camera::UpdateSwitchSection(D3DXVECTOR2 mainPos)
{
	int lastX = mainPos.x - (mWidth / 4);
	this->SetPosition(lastX, mPosition.y);
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