#ifndef _SCENE_H
#define _SCENE_H
#include <d3dx9.h>
#include "KeyEventHandler.h"
#include <string>
using namespace std;
class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	string sceneFilePath;

public:
	CScene() {}
	CScene(int id, string filePath);

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
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

