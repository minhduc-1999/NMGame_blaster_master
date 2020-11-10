#include "Sprites.h"

//class CSprite
CSprite::CSprite(int id, int left, int top, int right, int bottom,
	LPDIRECT3DTEXTURE9 texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = texture;
}

void CSprite::Draw(float x, float y, int dir)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, dir);
}

//class CSpriteManager
CSpriteManager* CSpriteManager::__instance = NULL;

CSpriteManager* CSpriteManager::GetInstance()
{
	if (__instance == NULL) __instance = new CSpriteManager();
	return __instance;
}

void CSpriteManager::Add(int id, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, texture);
	sprites[id] = s;
}

LPSPRITE CSpriteManager::Get(int id)
{
	return sprites[id];
}

//class Canimation frame
CAnimationFrame::CAnimationFrame(LPSPRITE sprite, int time) 
{
	this->sprite = sprite;
	this->time = time;
}

DWORD CAnimationFrame::GetTime() {
	return time;
}

LPSPRITE CAnimationFrame::GetSprite()
{
	return sprite;
}

//class CAnimation
void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0)
		t = this->defaultTime;

	LPSPRITE sprite = CSpriteManager::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int dir)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}
	if (currentFrame == frames.size() - 1)
		_isCompleted = true;
	frames[currentFrame]->GetSprite()->Draw(x, y, dir);
}

void CAnimation::RenderFrame(int frameID, float x, float y, int dir)
{
	currentFrame = frameID;
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}
	if (currentFrame == frames.size() - 1)
		_isCompleted = true;
	frames[currentFrame]->GetSprite()->Draw(x, y, dir);
}

void CAnimation::RenderStartByFrame(int frameID, float x, float y, int dir)
{
	currentFrame = frameID;
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}
	if (currentFrame == frames.size() - 1)
		_isCompleted = true;
	frames[currentFrame]->GetSprite()->Draw(x, y, dir);
}

//class CAnimationManager
CAnimationManager* CAnimationManager::__instance = NULL;

CAnimationManager* CAnimationManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new CAnimationManager();
	return __instance;
}

void CAnimationManager::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimationManager::Get(int id)
{
	return animations[id];
}