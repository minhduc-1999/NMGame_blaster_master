#pragma once
#include "Scene.h"
#include "Sprites.h"

#define INTRO_SPAN_MIN	6000

class IntroScene : public CScene
{
private:
	LPANIMATION_SET introSets;
	int current_intro;
	DWORD start;
public:
	IntroScene(int id, string filePath, int type, D3DXVECTOR3 bg);
	virtual void Load(D3DXVECTOR2 tlPos);
	virtual void Unload();
	virtual int Update(DWORD dt);
	virtual void Render();
	void NextIntro();
	virtual void _ParseSection_ANIMATIONS(string line);
};

class IntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	IntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};