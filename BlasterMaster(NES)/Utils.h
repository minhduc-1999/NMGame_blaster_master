#pragma once

#include <Windows.h>

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void DebugOut(const char* szFormat, ...);
//
vector<string> split(string line, string delimeter = "\t");
//wstring ToWSTR(string st);
//
//LPCWSTR ToLPCWSTR(string st);
struct Rect
{
	float left, top, right, bottom;
};

bool CheckIfBound(Rect b1, Rect b2);

