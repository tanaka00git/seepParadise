#pragma once
#include <Windows.h>

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static POINT m_MousePos;

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE keyCode);
	static bool GetKeyTrigger(BYTE keyCode);
	static POINT GetMousePosition();
	static bool IsMouseClickInArea(int startX, int startY, int endX, int endY);
};