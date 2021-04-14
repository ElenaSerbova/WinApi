#include "Windows.h"
#include "windowsx.h"
#include "tchar.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WCHAR className[] = L"FirstWindow";

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;

	wndClass.lpfnWndProc = WindowProc;

	if (!RegisterClassEx(&wndClass)) {
		return 0;
	}

	HWND hWnd = CreateWindowEx(
		0,
		className,
		L"My first window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, //x,y
		CW_USEDEFAULT, CW_USEDEFAULT, //width, height
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); //WM_QUIT
		break;

	case WM_LBUTTONDOWN:
		if((wParam & MK_CONTROL) != 0)
			SetWindowText(hwnd, L"Left button down with ctrl");
		else
			SetWindowText(hwnd, L"Left button down");
		break;

	case WM_LBUTTONUP:
		SetWindowText(hwnd, L"Left button up");
		break;

	case WM_LBUTTONDBLCLK:
		SetWindowText(hwnd, L"Left button double click");
		break;

	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		WCHAR str[20];
		wsprintf(str, L"x: %4d, y: %4d", x, y);

		SetWindowText(hwnd, str);
		break;
	}	

	case WM_RBUTTONUP:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

		WCHAR str[50];
		wsprintf(str, L"left: %d, top: %d, right: %d, bottom: %d", rect.left, rect.top, rect.right, rect.bottom);
		
		MessageBox(hwnd, str, L"Get window rect", MB_OK);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		MoveWindow(hwnd, rect.left + 50, rect.top + 50, width + 50, height + 50, true);
		break;
	}

	case WM_MBUTTONUP:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);

		WCHAR str[50];
		wsprintf(str, L"left: %d, top: %d, right: %d, bottom: %d", rect.left, rect.top, rect.right, rect.bottom);

		MessageBox(hwnd, str, L"Get client rect", MB_OK);
		
		break;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
