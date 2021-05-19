#include "MyDialog.h"

CMyDialog* CMyDialog::ptr=NULL;
HHOOK hHook;

CMyDialog::CMyDialog(void)
{
	ptr=this;
}

CMyDialog::~CMyDialog(void)
{
	
}

void CMyDialog::OnInitDialog(HWND h)
{
	hDialog = h;
	hStatic = GetDlgItem(hDialog, IDC_STATIC1);
}

LRESULT CALLBACK KeyboardProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	/*
		idCode - если параметр равен HC_ACTION, то процедура должна обработать HOOK,
		в противном случае (если < 0) сообщение нужно передать функции CallNextHookEx 
		для дальнейшей обработки, и возвратить значение, полученное от этой функции.
		wParam - код виртуальной клавиши
		lParam - кратность повторения, скэн-код, 31 бит - если нажата, то 1, иначе 0. 
	*/
	if(HC_ACTION == idCode)
	{
		HWND hWnd=GetFocus();
		static char buf[40],text[65536];
		GetClassName(hWnd,buf,40);
		if(lstrcmpi(buf,"EDIT") == 0 && (lParam & 0x80000000)) // клавиша отпущена!!!
		{
			if(wParam<=32) // непечатаемый символ
			{
				wsprintf(buf,"0X%X",wParam);
			}
			else
			{
				if(!(wParam>='A'&&wParam<='Z'))
					wsprintf(buf,"%c",wParam);
				else
				{
					short shift=GetKeyState(VK_SHIFT); // если старший бит равен 1, то клавиша нажата
					short caps=GetKeyState(VK_CAPITAL); // если младший бит равен 1, то клавиша переключена
					if((shift&0x8000) &&!(caps&1) || !(shift&0x8000)&&(caps&1))
					{
						wsprintf(buf,"%c",wParam);
					}
					else
					{
						wsprintf(buf,"%c",tolower(wParam));
					}
				}
			}
			lstrcat(text,buf);
			HWND hDialog = GetParent(hWnd);
			HWND hSt = GetDlgItem(hDialog, IDC_STATIC1);
			SetWindowText(hSt, text);
		}
	}
	return CallNextHookEx(hHook,idCode,wParam,lParam);
}		

void CMyDialog::OnCommand(WPARAM wp, LPARAM lp)
{
	if(LOWORD(wp)==IDC_BUTTON1)
	{
		hHook=SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL /* для собственного потока */, GetCurrentThreadId());
	}
	else if(LOWORD(wp)==IDC_BUTTON2)
	{
		UnhookWindowsHookEx(hHook);
	}
}

BOOL CALLBACK CMyDialog::DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
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