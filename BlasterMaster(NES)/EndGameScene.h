#pragma once
#include "Scene.h"
#include "Panel.h"
#include "Sprites.h"
#include <unordered_map>
using namespace std;

#define SCENE_SECTION_DESC	8
class EndGameScene : public CScene
{
private:
	int current_period;
	unordered_map<int, Panel*> panels;
public:
	EndGameScene(int id, string filePath, int type, D3DXVECTOR3 bg);
	virtual void Load(D3DXVECTOR2 tlPos);
	virtual void Unload();
	virtual int Update(DWORD dt);
	virtual void Render();
	void NextIntro();
	void _ParseSection_DESCRIPTION(string line);
};

class EndGameKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {};
	virtual void OnKeyUp(int KeyCode) {};
	EndGameKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};