#include "CGate.h"

CGate::CGate(float x, float y, int sectionId, D3DXVECTOR2 tele, int width, int height) :CStaticGameObject(x, y)
{
    SetSize(width, height);
	desTelePos = tele;
	nextSectionID = sectionId;
}

void CGate::Render()
{
    /*_tile.at(0)->Draw(x - 8, y - 8, -1);
    _tile.at(1)->Draw(x + 8, y - 8, -1);
    _tile.at(2)->Draw(x - 8, y + 8, -1);
    _tile.at(3)->Draw(x + 8, y + 8, -1);*/
	/*int row = height / 16;
	int col = width / 16;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			int index = (i % row) * col + j % col; 
			_tile.at(index)->Draw(x, y, -1);
		}*/
	//RenderBoundingBox();
}

Rect CGate::GetBound()
{
	Rect bound;

	bound.left = x - width / 2 + 1;
	bound.right = bound.left + width - 2;
	bound.top = y - height / 2 + 1;
	bound.bottom = bound.top + height - 2;

	return bound;
}
