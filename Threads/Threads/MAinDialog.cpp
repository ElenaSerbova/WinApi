#include "MAinDialog.h"

DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
	HWND hList = (HWND)lpParameter;
	for (int i = 0; i < 15; i++)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Работает первый поток")));
		Sleep(500);
	}
	SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Первый поток завершает работу")));
	return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
	HWND hList = (HWND)lpParameter;
	for (int i = 0; i < 20; i++)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Работает второй поток")));
		Sleep(500);
	}
	SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Второй поток завершает работу")));
	return 0;
}

DWORD WINAPI ThreadProc3(LPVOID lpParameter)
{
	HWND hList = (HWND)lpParameter;
	for (int i = 0; i < 10; i++)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Работает третий поток")));
		Sleep(500);
	}
	SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Третий поток завершает работу")));
	return 0;
}

void MainDialog::OnInitDialog(HWND hwnd)
{
	HWND hList = GetDlgItem(hwnd, IDC_LIST1);

	CreateThread(NULL, 0, ThreadProc1, hList, 0, NULL);
	CreateThread(NULL, 0, ThreadProc2, hList, 0, NULL);
	CreateThread(NULL, 0, ThreadProc3, hList, 0, NULL);
}