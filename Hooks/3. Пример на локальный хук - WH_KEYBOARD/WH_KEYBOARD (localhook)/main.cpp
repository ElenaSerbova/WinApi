#include "MyDialog.h"


int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpszCmdLine,int nCmdShow)
{
	CMyDialog dlg;
	return DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG1),NULL,CMyDialog::DlgProc);
}

