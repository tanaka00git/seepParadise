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
	GetKeyboardState(m_KeyState);

	GetCursorPos(&m_MousePos);
	ScreenToClient(GetForegroundWindow(), &m_MousePos);
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
	
	//ƒ}ƒEƒX‚ª”ÍˆÍ“à‚É‚ ‚é‚©‚ð”»’è
	bool isInsideX = (mousePos.x >= startX) && (mousePos.x <= endX);
	bool isInsideY = (mousePos.y >= startY) && (mousePos.y <= endY);
	return isInsideX && isInsideY;
}
