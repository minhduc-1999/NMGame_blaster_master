#include "SelectPanel.h"

SelectPanel::SelectPanel(float x, float y, float vx, float vy, int w, int h, unordered_map<int, CSprite*> sp)
	:Panel(x, y, vx, vy)
{
	this->width = w;
	this->height = h;
	this->sprites = sp;
	this->current_select = 0;
}
void SelectPanel::Update(unsigned long dt)
{
}
void SelectPanel::Render()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		int ty = (y - 25) + i * (16 + 20);
		if( i == current_select)
			sprites.at(i)->Draw(x, ty, -1, D3DCOLOR_XRGB(228, 0, 88), 1.0f, 1.0f);
		else
			sprites.at(i)->Draw(x, ty, -1, 255);
	}
}
bool SelectPanel::IsDone()
{
	if (done)
		return true;
	return false;
}
void SelectPanel::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_UP)
	{
		current_select = (current_select - 1 + sprites.size()) % sprites.size();
	}
	else if (KeyCode == DIK_DOWN)
	{
		current_select = (current_select + 1) % sprites.size();
	}
	else if (KeyCode == DIK_RETURN)
	{
		if(current_select == 0)
			CGame::GetInstance()->Notify(3);
	}
}
