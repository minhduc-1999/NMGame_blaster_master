#ifndef _KEYEVENTHANDLER_H
#define _KEYEVENTHANDLER_H
#include <Windows.h>

/*
	Abstract class to define keyboard event handlers
*/
class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE* state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};
typedef CKeyEventHandler* LPKEYEVENTHANDLER;
#endif // !_KEYEVENTHANDLER_H

