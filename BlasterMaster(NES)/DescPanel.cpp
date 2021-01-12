#include "DescPanel.h"
#include "Sprites.h"

DescPanel::DescPanel(float x, float y, float vx, float vy) : Panel(x, y, vx, vy)
{
	width = 128;
	height = 300;
}
void DescPanel::PushDesc(string str)
{
	desc.push_back(str);
}
void DescPanel::Update(unsigned long dt)
{
	if (!done)
	{
		this->y += vy * dt;
		this->x += vx * dt;
		if (y + height <= 0)
			y = 224;
	}
	
}
void DescPanel::Render()
{

	for (int i = 0; i < desc.size(); i++)
	{
		string line = desc.at(i);
		int tab = 0;
		for (int j = 0; j < line.size(); j++)
		{
			if (line[j] == '\t')
			{
				tab++;
				continue;
			}
			if (line[j] != ' ')
			{
				int px = x + j * 8 + 4 + 20 * tab;
				int py = y + i * 15 + 4;
				int sprite = line[j];
				if (py + CHAR_HEIGHT <= 0 || py > 224)
					continue;
				CSpriteManager::GetInstance()->Get(sprite)->Draw(px, py, -1, 255);
			}

		}
	}
}