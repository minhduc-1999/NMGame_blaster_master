#pragma once

#include <d3d9.h>
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <unordered_map>
#include "Scene.h"
#include "Camera.h"

using namespace std;
#define KEYBOARD_BUFFER_SIZE 1024
#define DIRECTINPUT_VERSION 0x0800

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)

class CGame
{
	static CGame* __instance;
	HWND hwnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;

	LPDIRECT3DSURFACE9 backbuffer = NULL;
	LPD3DXSPRITE spriteHander = NULL;

	int backbufferWidth = 0;
	int backbufferHeight = 0;

	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 didv;

	BYTE keyStates[256];
	DIDEVICEOBJECTDATA	keyEvents[KEYBOARD_BUFFER_SIZE];

	LPKEYEVENTHANDLER keyHandler;

	Camera* camera;
	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
public:
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int dir);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddev; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return this->backbuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHander; }

	int GetBackBufferWidth() { return backbufferWidth; }
	int GetBackBufferHeight() { return backbufferHeight; }

	static CGame* GetInstance();

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Render();
	void Update(DWORD dt);

	void UpdateCamera(D3DXVECTOR2 mainPos, D3DXVECTOR2 mapPos, D3DXVECTOR2 mapDimen) { camera->Update(mainPos, mapPos, mapDimen); }
	D3DXVECTOR2 GetCamPos() { return camera->GetPosition(); }
	Rect GetCamBound() { return camera->GetBound(); }
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	~CGame();

	void Load(LPCSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);
	void SwitchSection(int section_id, D3DXVECTOR2 tlPos);
	void UpdateSwitchSectionCamera(DWORD dt, int nx, D3DXVECTOR2 desMapPos, D3DXVECTOR2 desMapDimen) { camera->UpdateSwitchSection(dt, nx, desMapPos, desMapDimen); }
};