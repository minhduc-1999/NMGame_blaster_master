#pragma once
#include <string>
#include "Utils.h"
#include "Panel.h"

#define CHAR_HEIGHT 8
using namespace std;

class DescPanel :public Panel
{
private:
	vector<string> desc;
public:
	DescPanel(float x, float y, float vx, float vy);
	void Update(unsigned long dt);
	void Render();
	void PushDesc(string str);
};