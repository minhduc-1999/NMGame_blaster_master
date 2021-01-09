#ifndef _SCENE_H
#define _SCENE_H
#include <d3dx9.h>
#include "KeyEventHandler.h"
#include <string>
#include "SaveData.h"

using namespace std;

class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	int type;
	string sceneFilePath;
	SaveData* saveData;
public:
	CScene() {}
	CScene(int id, string filePath);

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load(D3DXVECTOR2 tlPos) = 0;
	virtual void Unload() = 0;
	virtual int Update(DWORD dt) = 0;
	virtual void Render() = 0;

	virtual void SwitchSection(int section_id, D3DXVECTOR2 telePos) = 0;
	SaveData* GetSaveData() {
		return saveData;
	}

	void SetSaveData(SaveData* data) { this->saveData = data; }

	void SetType(int type) { this->type = type; }
	int GetType() { return type; }
};
typedef CScene* LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};
#endif // !_SCENE_H

