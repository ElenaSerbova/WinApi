#include "MyDialog.h"

CMyDialog* CMyDialog::ptr=NULL;

CMyDialog::CMyDialog(void)
{
	ptr=this;
}


CMyDialog::~CMyDialog(void)
{
	
}

void CMyDialog::OnInitDialog(HWND h)
{
	hDialog=h;
	hButton1 = GetDlgItem(hDialog, IDC_BUTTON1);
	hButton2 = GetDlgItem(hDialog, IDC_BUTTON2);
}

HMODULE hModule;
typedef BOOL (*INSTALL)();
typedef BOOL (*REMOVE)();

void CMyDialog::OnCommand(WPARAM wp, LPARAM lp)
{
	if(LOWORD(wp) == IDC_BUTTON1)
	{
		hModule = LoadLibrary("WH_GETMESSAGE.dll");
		if (!hModule) 
			return;
		INSTALL f = (INSTALL)GetProcAddress(hModule,"InstallHook");
		if (!f) 
		{
			MessageBox(hDialog,"Error",0,0);
			FreeLibrary(hModule);
			return;
		}
		f();
		EnableWindow(hButton1, 0);
		EnableWindow(hButton2, 1);
	}
	else if(LOWORD(wp) == IDC_BUTTON2)
	{
		REMOVE f = (REMOVE)GetProcAddress(hModule,"RemoveHook");
		if (!f) 
		{
			MessageBox(hDialog,"Error",0,0);
			FreeLibrary(hModule);
			return;
		}
		f();
		FreeLibrary(hModule);
		EnableWindow(hButton1, 1);
		EnableWindow(hButton2, 0);
	}

}


BOOL CALLBACK CMyDialog::DlgProc(HWND hWnd,UINT mes,WPARAM wp,LPARAM lp)
{
	switch(mes)
	{
		case WM_CLOSE:
			EndDialog(hWnd,0);
			break;
		case WM_INITDIALOG:
			ptr->OnInitDialog(hWnd);
			break;
		case WM_COMMAND:
			ptr->OnCommand(wp,lp);
			break;
	}
	return 0;
}