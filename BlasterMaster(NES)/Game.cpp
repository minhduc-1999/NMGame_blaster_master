﻿#include "Game.h"
#include "CTestScene.h"
#include <fstream>
#include "Textures.h"

CGame* CGame::__instance = NULL;

void CGame::Init(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		//DebugOut("[ERROR] Error initializing Direct3D!\n");
		return;
	}

	this->hwnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);

	d3dpp.BackBufferWidth = r.right;
	d3dpp.BackBufferHeight = r.bottom;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	backbufferWidth = d3dpp.BackBufferWidth;
	backbufferHeight = d3dpp.BackBufferHeight;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if (d3ddev == NULL)
	{
		//DebugOut("[ERROR] Create Device failed!\n");
		return;
	}

	d3ddev->GetBackBuffer(0, 0,
		D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	HRESULT result;
	result = D3DXCreateSprite(d3ddev, &spriteHander);
	if (result != D3D_OK)
	{
		//DebugOut("[ERROR] Error creating Sprite handler!\n");
		return;
	}
	//DebugOut("[INFO] Init done!\n");
	camera = new Camera(screen_width, screen_height);
}

void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int dir)
{
	D3DXVECTOR2 camPos = GetCamPos();
	int viewPortY = y - camPos.y;
	int viewPortX = x - camPos.x;
	D3DXVECTOR3 pCenter((right - left) / 2, (bottom - top) / 2, 0);

	RECT r;
	r.left = left;
	r.right = right;
	r.top = top;
	r.bottom = bottom;
	if (dir == -1)
	{
		D3DXVECTOR3 p(viewPortX, viewPortY, 0);
		spriteHander->Draw(texture, &r, &pCenter, &p, D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		// ma trận lưu thông tin về tỉ lệ
		D3DXVECTOR3 p(-viewPortX, viewPortY, 0);

		D3DXMATRIX matScale;

		// khởi tạo ma trận phóng to theo trục Ox -1 lần, trục Oy 1 lần.
		D3DXMatrixScaling(&matScale, -1.0f, 1.0f, .0f);

		// thực hiện việc chuyển đổi.
		spriteHander->SetTransform(&matScale);
		spriteHander->Draw(texture, &r, &pCenter, &p, D3DCOLOR_XRGB(255, 255, 255));
		
		//reset transform
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, .0f);
		spriteHander->SetTransform(&matScale);
	}
}

CGame* CGame::GetInstance()
{
	if (__instance == NULL)
		__instance = new CGame();
	return __instance;
}

void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK)
	{
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		return;
	}
}

int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				//DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE*)&keyStates);

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

CGame::~CGame()
{
	if (spriteHander != NULL) spriteHander->Release();
	if (backbuffer != NULL) backbuffer->Release();
	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

void CGame::Render()
{
	if (d3ddev->BeginScene())
	{
		// Clear back buffer with a color
		d3ddev->ColorFill(backbuffer, NULL, BACKGROUND_COLOR);

		spriteHander->Begin(D3DXSPRITE_ALPHABLEND);

		scenes[current_scene]->Render();

		spriteHander->End();
		d3ddev->EndScene();
	}

	// Display back buffer content to the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);
	
}

void CGame::Update(DWORD dt)
{
	scenes[current_scene]->Update(dt);
}

void CGame::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) ||
		tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut("[ERROR] Unknown game setting %s\n", tokens[0]);
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	string path = tokens[1];

	LPSCENE scene = new CTestScene(id, path);
	scenes[id] = scene;
}

/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCSTR gameFile)
{
	DebugOut("[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	string line;
	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (getline(f, line))
	{
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut("[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}

void CGame::SwitchScene(int scene_id)
{
	DebugOut("[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();

	CTextureManager::GetInstance()->Clear();
	CSpriteManager::GetInstance()->Clear();
	CAnimationManager::GetInstance()->Clear();

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	s->Load();
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	//SetKeyHandler(s->GetKeyEventHandler());
}

void CGame::SwitchSection(int section_id, D3DXVECTOR2 tlPos)
{
	scenes[current_scene]->SwitchSection(section_id, tlPos);
}

void CGame::UpdateSwitchSectionCamera(D3DXVECTOR2 mainPos)
{
	camera->UpdateSwitchSection(mainPos);
}


