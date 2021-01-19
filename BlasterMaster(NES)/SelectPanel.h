#pragma once
#include "Panel.h"
#include "Sprites.h"

class SelectPanel :public Panel
{
	int current_select;
	unordered_map<int, CSprite*> sprites;
public:
	SelectPanel(float x, float y, float vx, float vy, int w, int h, unordered_map<int, CSprite*> sp);
	void Update(unsigned long dt);
	void Render();
	bool IsDone();
	void KeyState(BYTE* states) {};
	void OnKeyDown(int KeyCode) ;
	void OnKeyUp(int KeyCode) {};
};