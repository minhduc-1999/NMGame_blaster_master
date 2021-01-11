#ifndef _SCENE_H
#define _SCENE_H
#include <d3dx9.h>
#include "KeyEventHandler.h"
#include <string>
#include "SaveData.h"
#include <d3d9types.h>

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_SECTION	6
#define SCENE_SECTION_SOUND	7

class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	int type;
	string sceneFilePath;
	SaveData* saveData;
	D3DCOLOR background;
public:
	CScene(int id, string filePath, int t, D3DXVECTOR3 background);

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load(D3DXVECTOR2 tlPos) = 0;
	virtual void Unload() = 0;
	virtual int Update(DWORD dt) = 0;
	virtual void Render() = 0;

	virtual void SwitchSection(int section_id, D3DXVECTOR2 telePos) {};
	SaveData* GetSaveData() {
		return saveData;
	}

	void SetSaveData(SaveData* data) { this->saveData = data; }

	int GetType() { return type; }
	D3DCOLOR GetBackground() { return background; }

	virtual void _ParseSection_TEXTURES(string line);
	virtual void _ParseSection_SPRITES(string line);
	virtual void _ParseSection_ANIMATIONS(string line);
	virtual void _ParseSection_ANIMATION_SETS(string line);
	virtual void _ParseSection_SOUND(string line);
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

