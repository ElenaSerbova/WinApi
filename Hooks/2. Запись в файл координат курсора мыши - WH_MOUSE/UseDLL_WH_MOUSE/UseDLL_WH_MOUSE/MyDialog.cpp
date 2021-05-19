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
}

void CMyDialog::OnCommand(WPARAM wp, LPARAM lp)
{
	if(LOWORD(wp) == IDC_BUTTON1)
	{
		if (!installhook())
		{
			MessageBox(hDialog, "Хук не был установлен!!!", "HOOK", MB_OK | MB_ICONSTOP);
		}
	}
	else if(LOWORD(wp) == IDC_BUTTON2)
	{
		if (!removehook())
		{
			MessageBox(hDialog, "Хук не был удален!!!", "HOOK", MB_OK | MB_ICONSTOP);
		}
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