#include "Camera.h"


#define SWITCH_SECTION_CAM_SPEED 0.2f

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
	//cal cam X
	Rect lockBound = GetLockBound();
	int lastX = mainPos.x - (mWidth / 2), lastY;
	if (lastX < mapPos.x)
	{
		lastX = mapPos.x;
	}
	else
	{
		if (lastX > mapPos.x + mapDimen.x - mWidth)
			lastX = mapPos.x + mapDimen.x - mWidth;
	}
	//call cam Y
	if (mainPos.y >= lockBound.top && mainPos.y <= lockBound.bottom)
	{
		this->SetPosition(lastX, mPosition.y);
		return;
	}
	else if (mainPos.y < lockBound.top)
	{
		lastY = mainPos.y - (lockBound.top - mPosition.y);
	}
	else
		lastY = mainPos.y - (mPosition.y + mHeight - lockBound.bottom - 1);
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
	//DebugOut("[UPDATE CAM]\t%f\t%f\n", mPosition.x, mPosition.y);
}

void Camera::UpdateSwitchSection(DWORD dt, int nx, D3DXVECTOR2 desMapPos, D3DXVECTOR2 desMapDimen)
{
	float dx = nx * SWITCH_SECTION_CAM_SPEED * dt;
	float lastX = this->mPosition.x + dx;
	if (nx == 1)
	{
		if (lastX > desMapPos.x)
			lastX = desMapPos.x;
	}
	else
	{
		if (lastX + mWidth < desMapPos.x + desMapDimen.x)
			lastX = desMapPos.x + desMapDimen.x - mWidth;
	}
	//DebugOut("[Delta x Cam trans]\t%f, Dir: %d, dt: %d\n", dx, nx, dt);
	this->SetPosition(lastX, this->mPosition.y);
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

Rect Camera::GetLockBound()
{
	Rect bound;

	bound.left = mPosition.x + mWidth / 2.0f;
	bound.right = bound.left;
	bound.top = mPosition.y + mHeight / 4.0f;
	bound.bottom = bound.top + mHeight / 2.5f;

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