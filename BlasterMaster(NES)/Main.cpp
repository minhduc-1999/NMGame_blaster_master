#include "ResourceManager.h"

#define WINDOW_CLASS_NAME "WindowClassName"
#define WINDOW_TITLE "NES-BlasterMaster"
#define WINDOW_ICON_PATH "BlasterMaster.ico"

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 280

#define MAX_FRAME_RATE 60




CGame* game;
ResourceManager* resourceManager;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

ATOM RegisterClass(HINSTANCE hInstance)
{
	/*WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);*/

	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	//fill the struct with info
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;
	//set up the window with the class info
	return RegisterClassEx(&wc);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{

	RegisterClass(hInstance);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,//WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		//DebugOut("[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//SetDebugWindow(hWnd);

	return hWnd;
}


void Update(DWORD dt)
{
	//enemy1->Update(dt);
	game->Update(dt);
}

/*
	Render a frame
*/
void Render()
{
	//CGame* game = CGame::GetInstance();
	//LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	//LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	//LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	//if (d3ddv->BeginScene())
	//{
	//	// Clear back buffer with a color
	//	d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

	//	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	//	//enemy1->Render();
	//	
	//	//DebugOutTitle("01 - Sprite %0.1f %0.1f", enemy1->GetX(), enemy1->GetY());

	//	//
	//	// TEST SPRITE DRAW
	//	//

	//	/*
	//	CTextures *textures = CTextures::GetInstance();

	//	D3DXVECTOR3 p(20, 20, 0);
	//	RECT r;
	//	r.left = 274;
	//	r.top = 234;
	//	r.right = 292;
	//	r.bottom = 264;
	//	spriteHandler->Draw(textures->Get(ID_TEX_ENEMY), &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
	//	*/

	//	spriteHandler->End();
	//	d3ddv->EndScene();
	//}

	//// Display back buffer content to the screen
	//d3ddv->Present(NULL, NULL, NULL, NULL);
	game->Render();
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	game = CGame::GetInstance();
	game->Init(hWnd);

	game->InitKeyboard();

	//resourceManager = ResourceManager::GetInstance();
	//resourceManager->LoadResource();

	game->Load("Gameworld.txt");

	Run();

	return 0;
}