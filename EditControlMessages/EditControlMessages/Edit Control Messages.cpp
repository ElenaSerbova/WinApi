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
	int row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0); // Получим номер строки, в которой расположен курсор
	
	DWORD Start, End;
	SendMessage(hEdit, EM_GETSEL, (WPARAM)&Start, (LPARAM)&End); // Получим границы выделения текста
	int col = Start - SendMessage(hEdit, EM_LINEINDEX, row, 0); // Получим номер первого символа указанной строки
	
	HWND hParent = GetParent(hEdit); // Получим дескриптор родительского окна (диалога)
	TCHAR buffer[50] = {0};
	wsprintf(buffer, TEXT("Строка %d, Столбец %d"), row+1, col+1); 
	SetWindowText(hParent, buffer); // установим в заголовок главного окна текущие координаты курсора
	
	SetFocus(hEdit); // переведём фокус ввода на Edit Control
}

void EnableDisableButton()
{
	DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0); // Получим границы выделения текста
	WORD wBeginPosition = LOWORD(dwPosition);
	WORD wEndPosition = HIWORD(dwPosition);
	if(wEndPosition != wBeginPosition) // Выделен ли текст?
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

	if(IsClipboardFormatAvailable(CF_TEXT)) // Имеется ли текст в буфере обмена?
		EnableWindow(hPaste, 1);
	else 
		EnableWindow(hPaste, 0);

	if(SendMessage(hEdit, EM_CANUNDO, 0, 0)) // Существует ли возможность отмены последнего действия?
		EnableWindow(hUndo, 1);
	else 
		EnableWindow(hUndo, 0);

	int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0); // Определим длину текста в Edit Control
	if(length != wEndPosition - wBeginPosition) // Выделен ли весь текст в Edit Control?
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
	return CallWindowProc(OriginalProc, hWnd, message, wParam, lParam); // Вызов стандартного обработчика сообщений
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
			OriginalProc = (WNDPROC) SetWindowLong(hEdit, GWL_WNDPROC, LONG(EditProc)); // переопределим оконную процедуру текстового поля
			SendMessage(hEdit, WM_POSITION, 0, 0); // отправка пользовательского сообщения для определения текущих координат курсора
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_UNDO: 
					SendMessage(hEdit, WM_UNDO, 0, 0); // отменить последнее действие
					SendMessage(hEdit, WM_POSITION, 0, 0); // определить текущие координаты курсора
					break;
				case ID_COPY: 
					SendMessage(hEdit, WM_COPY, 0, 0); // скопировать выделенный фрагмент текста
					break;
				case ID_PASTE: 
					SendMessage(hEdit, WM_PASTE, 0, 0); // вставить текст в Edit Control из буфера обмена
					SendMessage(hEdit, WM_POSITION, 0, 0); // определить текущие координаты курсора
					break;
				case ID_CUT: 
					SendMessage(hEdit, WM_CUT, 0, 0); // вырезать выделенный фрагмент текста
					SendMessage(hEdit, WM_POSITION, 0, 0); // определить текущие координаты курсора
					break;
				case ID_DELETE: 
					SendMessage(hEdit, WM_CLEAR, 0, 0); // удалить выделенный фрагмент текста
					SendMessage(hEdit, WM_POSITION, 0, 0); // определить текущие координаты курсора
					break;
				case ID_SELECTALL:
					SendMessage(hEdit, EM_SETSEL, 0, -1); // выделить весь текст в Edit Control
					SendMessage(hEdit, WM_POSITION, 0, 0); // определить текущие координаты курсора
					break;
			}
			return TRUE;
	}
	return FALSE;
}