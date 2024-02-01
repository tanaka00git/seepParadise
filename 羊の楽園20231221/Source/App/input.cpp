#include "..\App\main.h"
#include "..\App\input.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
POINT Input::m_MousePos;

void Input::Init()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
	m_MousePos = { 0, 0 };
}

void Input::Uninit()
{
}

void Input::Update()
{
	memcpy(m_OldKeyState, m_KeyState, 256);

	// マウス位置の取得
	POINT rawMousePos;
	GetCursorPos(&rawMousePos);
	ScreenToClient(GetForegroundWindow(), &rawMousePos);

	// マウス位置の更新
	m_MousePos.x = rawMousePos.x;
	m_MousePos.y = rawMousePos.y;

	// キーボードの状態の更新
	GetKeyboardState(m_KeyState);
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

POINT Input::GetMousePosition()
{
	return m_MousePos;
}

bool Input::IsMouseClickInArea(int startX, int startY, int endX, int endY)
{
	POINT mousePos = Input::GetMousePosition();
	
	//マウスが範囲内にあるかを判定
	bool isInsideX = (mousePos.x >= startX) && (mousePos.x <= endX);
	bool isInsideY = (mousePos.y >= startY) && (mousePos.y <= endY);
	return isInsideX && isInsideY;
}
