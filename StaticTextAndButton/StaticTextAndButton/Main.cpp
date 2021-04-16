#include "Windows.h"
#include "Windowsx.h"
#include "tchar.h"
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WCHAR className[] = L"FirstWindow";

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
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

HWND hStaticCoords = NULL;
HWND hStaticImage = NULL;
HWND hButton1 = NULL;

void WindowCreateHandler(HWND hwnd, LPARAM lParam) 
{

	HINSTANCE hInst = GetModuleHandle(0);

	//Создаем элемент управления Static, на котором можно разместить текст или изображение
	hStaticCoords = CreateWindowEx(
		0,
		L"Static", //системный класс окна для статика
		L"Static text",
		WS_CHILD | WS_VISIBLE ,
		10, 10, 200, 20,
		hwnd,
		0,
		hInst,
		0);

	//Создаем элемент управления кнопка
	hButton1 = CreateWindowEx(
		0,
		L"Button", //системный класс окна для кнопки
		L"Click me",
		WS_CHILD | WS_VISIBLE,
		220, 5, 75, 25,
		hwnd,
		0, 
		hInst,
		0);

	//Создаем элемент управления Static, на котором будем размещать изображение
	hStaticImage = CreateWindowEx(
		0,
		L"Static",
		L"",
		WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, //SS_BITMAP стиль для изображений
		10, 40, 100, 50,
		hwnd,
		0,
		hInst,
		0);

	//Загрузка изображения из ресурсов
	HBITMAP hImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	//Отправка сообщения окну статика, чтобы он отрисовал изображение
	SendMessage(hStaticImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
}

void MouseMoveHandler(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	WCHAR str[50];
	wsprintf(str, L"mouse coords: {%4d, %4d}", x, y);

	SetWindowText(hStaticCoords, str);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//WM_COMMAND это сообщение отправляют элементы управления, 
		//уведомляя главное окно, что в них произошли какие-то события

		//Например кнопка уведомляет, что пользователь ее нажал. 

		//в lParam приходит дескриптор элемента управления.
	case WM_COMMAND:
		if((HWND)lParam == hButton1){
			MessageBox(hwnd, L"Button clicked", L"", MB_OK);
		}
		break;

	case WM_CREATE:
		WindowCreateHandler(hwnd, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		MouseMoveHandler(hwnd, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, L"left button down", L"", MB_OK);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
