#include "..\App\main.h"
#include "..\App\manager.h"
#include <thread>

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "seepParadise";
RECT g_WindowRect;				//�E�B���h�E�̋�`�̈��ۑ�����ϐ�
bool g_IsMouseCaptured = true;	//�}�E�X����������Ă��邩�ǂ����������ϐ�


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_Window;
HWND GetWindow(){ return g_Window; }

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);

		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}
	//�����̃V�[�h�l�ݒ�
	srand(static_cast<unsigned int>(time(nullptr)));

	Manager::Init();

	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { break; }
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				Manager::Update();
				Manager::Draw();
			}

			// �}�E�X�����̏�Ԃɂ���Đ����������邩�������邩�𔻒f
			if (g_IsMouseCaptured)
			{
				ClipCursor(&g_WindowRect);
			}
			else
			{
				ClipCursor(nullptr);
			}
		}
	}


	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		ShowCursor(FALSE);  // �}�E�X���\���ɂ���
		break;

	case WM_SIZE:
		// �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɐV�����E�B���h�E��`���擾
		GetClientRect(hWnd, &g_WindowRect);
		ClientToScreen(hWnd, (LPPOINT)&g_WindowRect.left);
		ClientToScreen(hWnd, (LPPOINT)&g_WindowRect.right);
		break;

	case WM_MOVE:
		// �E�B���h�E���ړ����ꂽ�Ƃ��ɐV�����E�B���h�E��`���擾
		GetClientRect(hWnd, &g_WindowRect);
		ClientToScreen(hWnd, (LPPOINT)&g_WindowRect.left);
		ClientToScreen(hWnd, (LPPOINT)&g_WindowRect.right);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_RBUTTONDOWN:
		// �E�N���b�N�Ń}�E�X�̐������������A�J�[�\����\��
		g_IsMouseCaptured = !g_IsMouseCaptured;
		if (g_IsMouseCaptured) { ShowCursor(FALSE); }
		else { ShowCursor(TRUE); }
		break;


	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int irand(int min, int max) { return min + rand() % (max - min + 1);}
float frand(void) { return (float)rand() / RAND_MAX; }