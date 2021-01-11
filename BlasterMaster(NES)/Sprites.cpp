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

void CSprite::Draw(float x, float y, int dir, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, dir, alpha);
}

void CSprite::DrawFlipY(float x, float y, int dir, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->DrawFlipY(x, y, texture, left, top, right, bottom, dir, alpha);
}

void CSprite::Draw(float x, float y, int dir, D3DCOLOR color, float scaleX, float scaleY)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, dir, color, scaleX, scaleY);
}

//class CSpriteManager
CSpriteManager* CSpriteManager::__instance = NULL;

void CSpriteManager::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

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
	this->color = NULL;
}

CAnimationFrame::CAnimationFrame(LPSPRITE sprite, int time, D3DCOLOR color)
{
	this->sprite = sprite;
	this->time = time;
	this->color = color;
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

void CAnimation::AddWithColor(int spriteId, D3DCOLOR color, DWORD time)
{
	int t = time;
	if (time == 0)
		t = this->defaultTime;

	LPSPRITE sprite = CSpriteManager::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, color, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int dir, int alpha)
{
	DWORD now = GetTickCount64();
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
	frames[currentFrame]->GetSprite()->Draw(x, y, dir, alpha);
}

void CAnimation::RenderFlipY(float x, float y, int dir, int alpha)
{
	DWORD now = GetTickCount64();
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
	frames[currentFrame]->GetSprite()->DrawFlipY(x, y, dir, alpha);
}

void CAnimation::Render(float x, float y, int dir)
{
	Render(x, y, dir, 255);
}

void CAnimation::RenderWithColor(float x, float y, int dir, float scaleX, float scaleY)
{
	DWORD now = GetTickCount64();
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
	D3DCOLOR color = frames[currentFrame]->GetColor();
	frames[currentFrame]->GetSprite()->Draw(x, y, dir, color, scaleX, scaleY);
}

void CAnimation::RenderFrame(int frameID, float x, float y, int dir, int alpha)
{
	currentFrame = frameID;
	DWORD now = GetTickCount64();
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
	frames[currentFrame]->GetSprite()->Draw(x, y, dir, alpha);
}

void CAnimation::RenderFrame(int frameID, float x, float y, int dir)
{
	RenderFrame(frameID, x, y, dir, 255);
}

void CAnimation::RenderStartByFrame(int frameID, float x, float y, int dir, int alpha)
{
	currentFrame = frameID;
	DWORD now = GetTickCount64();
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
	frames[currentFrame]->GetSprite()->Draw(x, y, dir, alpha);
}

void CAnimation::RenderStartByFrame(int frameID, float x, float y, int dir)
{
	RenderStartByFrame(frameID, x, y, dir, 255);
}

//class CAnimationManager
CAnimationManager* CAnimationManager::__instance = NULL;

CAnimationManager* CAnimationManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new CAnimationManager();
	return __instance;
}

void CAnimationManager::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

void CAnimationManager::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimationManager::Get(int id)
{
	return animations[id];
}

CAnimationSets* CAnimationSets::__instance = NULL;

CAnimationSets::CAnimationSets()
{

}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) 
		__instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	//if (ani_set == NULL)
		//DebugOut("[ERROR] Failed to find animation set id: %d\n", id);
	return ani_set;
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}