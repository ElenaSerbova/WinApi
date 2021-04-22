#include <windows.h>
#include "resource.h"

#define WM_POSITION WM_APP

HWND hEdit, hCopy, hCut, hPaste, hDelete, hUndo, hSelectAll;
WNDPROC OriginalProc = NULL;


BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc); 
}

void GetPosition()
{
	int row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0); // ������� ����� ������, � ������� ���������� ������
	
	DWORD Start, End;
	SendMessage(hEdit, EM_GETSEL, (WPARAM)&Start, (LPARAM)&End); // ������� ������� ��������� ������
	int col = Start - SendMessage(hEdit, EM_LINEINDEX, row, 0); // ������� ����� ������� ������� ��������� ������
	
	HWND hParent = GetParent(hEdit); // ������� ���������� ������������� ���� (�������)
	TCHAR buffer[50] = {0};
	wsprintf(buffer, TEXT("������ %d, ������� %d"), row+1, col+1); 
	SetWindowText(hParent, buffer); // ��������� � ��������� �������� ���� ������� ���������� �������
	
	SetFocus(hEdit); // �������� ����� ����� �� Edit Control
}

void EnableDisableButton()
{
	DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0); // ������� ������� ��������� ������
	WORD wBeginPosition = LOWORD(dwPosition);
	WORD wEndPosition = HIWORD(dwPosition);
	if(wEndPosition != wBeginPosition) // ������� �� �����?
	{
		EnableWindow(hCopy, 1);
		EnableWindow(hCut, 1);
		EnableWindow(hDelete, 1);
	}
	else
	{
		EnableWindow(hCopy, 0);
		EnableWindow(hCut, 0);
		EnableWindow(hDelete, 0);
	}

	if(IsClipboardFormatAvailable(CF_TEXT)) // ������� �� ����� � ������ ������?
		EnableWindow(hPaste, 1);
	else 
		EnableWindow(hPaste, 0);

	if(SendMessage(hEdit, EM_CANUNDO, 0, 0)) // ���������� �� ����������� ������ ���������� ��������?
		EnableWindow(hUndo, 1);
	else 
		EnableWindow(hUndo, 0);

	int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0); // ��������� ����� ������ � Edit Control
	if(length != wEndPosition - wBeginPosition) // ������� �� ���� ����� � Edit Control?
		EnableWindow(hSelectAll, 1);
	else 
		EnableWindow(hSelectAll, 0);	
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_POSITION ||  message == WM_LBUTTONDOWN || message == WM_LBUTTONUP ||
		message == WM_KEYDOWN || message == WM_KEYUP || message == WM_MOUSEMOVE && (wParam & MK_LBUTTON))
	{
		EnableDisableButton();
		GetPosition();
	}
	return CallWindowProc(OriginalProc, hWnd, message, wParam, lParam); // ����� ������������ ����������� ���������
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_INITDIALOG:
			hEdit = GetDlgItem(hWnd, IDC_EDIT1);
			hCopy = GetDlgItem(hWnd, ID_COPY);
			hCut = GetDlgItem(hWnd, ID_CUT);
			hPaste = GetDlgItem(hWnd, ID_PASTE);
			hDelete = GetDlgItem(hWnd, ID_DELETE);
			hSelectAll = GetDlgItem(hWnd, ID_SELECTALL);
			hUndo = GetDlgItem(hWnd, ID_UNDO);
			OriginalProc = (WNDPROC) SetWindowLong(hEdit, GWL_WNDPROC, LONG(EditProc)); // ������������� ������� ��������� ���������� ����
			SendMessage(hEdit, WM_POSITION, 0, 0); // �������� ����������������� ��������� ��� ����������� ������� ��������� �������
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_UNDO: 
					SendMessage(hEdit, WM_UNDO, 0, 0); // �������� ��������� ��������
					SendMessage(hEdit, WM_POSITION, 0, 0); // ���������� ������� ���������� �������
					break;
				case ID_COPY: 
					SendMessage(hEdit, WM_COPY, 0, 0); // ����������� ���������� �������� ������
					break;
				case ID_PASTE: 
					SendMessage(hEdit, WM_PASTE, 0, 0); // �������� ����� � Edit Control �� ������ ������
					SendMessage(hEdit, WM_POSITION, 0, 0); // ���������� ������� ���������� �������
					break;
				case ID_CUT: 
					SendMessage(hEdit, WM_CUT, 0, 0); // �������� ���������� �������� ������
					SendMessage(hEdit, WM_POSITION, 0, 0); // ���������� ������� ���������� �������
					break;
				case ID_DELETE: 
					SendMessage(hEdit, WM_CLEAR, 0, 0); // ������� ���������� �������� ������
					SendMessage(hEdit, WM_POSITION, 0, 0); // ���������� ������� ���������� �������
					break;
				case ID_SELECTALL:
					SendMessage(hEdit, EM_SETSEL, 0, -1); // �������� ���� ����� � Edit Control
					SendMessage(hEdit, WM_POSITION, 0, 0); // ���������� ������� ���������� �������
					break;
			}
			return TRUE;
	}
	return FALSE;
}