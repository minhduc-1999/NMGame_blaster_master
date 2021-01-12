#pragma once
#include "Scene.h"
#include "Sprites.h"

class EndGameScene : public CScene
{
private:
	LPANIMATION_SET endGameSets;
	int current_period;
	DWORD start;
public:
	EndGameScene(int id, string filePath, int type, D3DXVECTOR3 bg);
	virtual void Load(D3DXVECTOR2 tlPos);
	virtual void Unload();
	virtual int Update(DWORD dt);
	virtual void Render();
	void NextIntro();
	//virtual void _ParseSection_ANIMATIONS(string line);
};

class EndGameKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {};
	virtual void OnKeyUp(int KeyCode) {};
	EndGameKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};