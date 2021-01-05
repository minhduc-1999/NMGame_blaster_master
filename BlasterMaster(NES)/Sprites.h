#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Game.h"

using namespace std;
class CSprite
{
	int id;
	int left, top, right, bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(int id, int left, int top, int right, int bottom,
		LPDIRECT3DTEXTURE9 texture);
	void Draw(float x, float y, int dir, int alpha);
};

typedef CSprite* LPSPRITE;

class CSpriteManager
{
	static CSpriteManager* __instance;
	unordered_map<int, LPSPRITE> sprites;
public:
	void Add(int id, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom);
	LPSPRITE Get(int id);

	void Clear();
	static CSpriteManager* GetInstance();
};

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;
public:
	CAnimationFrame(LPSPRITE sprite, int time);
	DWORD GetTime();
	LPSPRITE GetSprite();
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	bool _isCompleted;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) 
	{ 
		this->defaultTime = defaultTime;
		lastFrameTime = -1; 
		currentFrame = -1;
		_isCompleted = false;
	}
	bool IsCompleted() { return _isCompleted; }
	void SetIsCompleted() {_isCompleted = true; }
	void ResetAnim() { currentFrame = -1; _isCompleted = false; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int dir);
	void RenderFrame(int frameID, float x, float y, int dir);
	void RenderStartByFrame(int frameID, float x, float y, int dir);
	void Render(float x, float y, int dir, int alpha);
	void RenderFrame(int frameID, float x, float y, int dir, int alpha);
	void RenderStartByFrame(int frameID, float x, float y, int dir, int alpha);
};

typedef CAnimation* LPANIMATION;
class CAnimationManager
{
	static CAnimationManager* __instance;
	unordered_map<int, LPANIMATION> animations;
public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	static CAnimationManager* GetInstance();
	void Clear();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
private:
	static CAnimationSets* __instance;
	unordered_map<int, LPANIMATION_SET> animation_sets;
public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets* GetInstance();
};