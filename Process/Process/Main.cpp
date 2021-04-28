#include <Windows.h>
#include "resource.h" 

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

HANDLE hProcess = NULL;

void OpenProcess(HWND hwnd)
{
	STARTUPINFO startupInfo = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION processInfo;

	WCHAR path[MAX_PATH] = L"C:\\Windows\\System32\\Notepad.exe";

	BOOL res = CreateProcess(path, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);

	if (res == FALSE)
	{
		MessageBox(hwnd, L"Error", L"", MB_OK);
	}
	else {
		hProcess = processInfo.hProcess;
	}
}

void CloseProcess()
{
	if (hProcess != NULL)
	{
		TerminateProcess(hProcess, 0);
	}	
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_OPEN) {
			OpenProcess(hwnd);
		}
		else if	(LOWORD(wParam) == IDC_CLOSE) {
			CloseProcess();
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}
