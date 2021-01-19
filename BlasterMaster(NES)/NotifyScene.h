#pragma once
#include "Scene.h"
#include "Panel.h"

class NotifyScene : public CScene
{
private:
	int current_panel;
	unordered_map<int, Panel*> panels;
public:
	NotifyScene(int id, string filePath, int type, D3DXVECTOR3 bg);
	Panel* GetCurPanel() { return panels[current_panel]; }
	virtual void Load(D3DXVECTOR2 tlPos);
	virtual void Unload();
	virtual int Update(DWORD dt);
	virtual void Render();
	void SetNotify(int mainLives);
};

class NotifySceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	NotifySceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};