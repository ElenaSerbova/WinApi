#include "Windows.h"
#include "tchar.h"
#include "resource.h"

HINSTANCE hInstance;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	::hInstance = hInstance;

	WCHAR className[] = L"FirstWindow";

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
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
	static HWND hButtonModal, hButtonModeless;

	switch (uMsg)
	{
	case WM_CREATE:

		hButtonModal = CreateWindowEx(
			0,
			L"Button",
			L"Open modal window",
			WS_VISIBLE | WS_CHILD,
			10, 10,
			200, 25,
			hwnd,
			NULL, 
			hInstance,
			0
		);

		hButtonModeless = CreateWindowEx(
			0,
			L"Button",
			L"Open modeless window",
			WS_VISIBLE | WS_CHILD,
			220, 10,
			200, 25,
			hwnd,
			NULL,
			hInstance,
			0
		);

		break;

	case WM_COMMAND:
		if ((HWND)lParam == hButtonModal) {

			//создает модальное окно
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);
		}
		else if ((HWND)lParam == hButtonModeless) {

			//создает немодальное окно
			HWND hDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);
			ShowWindow(hDialog, SW_SHOW);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}
