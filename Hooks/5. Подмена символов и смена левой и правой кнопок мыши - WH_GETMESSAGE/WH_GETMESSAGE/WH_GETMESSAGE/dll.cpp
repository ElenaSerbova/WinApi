// dll.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>

HHOOK hHook;
HINSTANCE hInstance;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hInstance = (HINSTANCE)hModule;
	}
    return TRUE;
}

LRESULT CALLBACK GetMsgProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	//WPARAM - PM_NOREMOVE - не удалено из очереди сообщений, PN_REMOVE - удалено из очереди сообщений
	//LPARAM - указатель на структуру MSG
	MSG*pMsg=(MSG*)lParam;
	if(idCode == HC_ACTION)
	{
		if(pMsg->message==WM_CHAR)
		{

			if(pMsg->wParam>='a'&&wParam<='z')
				pMsg->wParam-=32;
			else if(pMsg->wParam>='A'&&wParam<='Z')
				pMsg->wParam+=32;
		}
		else if(pMsg->message==WM_LBUTTONDOWN)
		{
			pMsg->message=WM_RBUTTONDOWN;
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{
			pMsg->message=WM_RBUTTONUP;
		}
		else if(pMsg->message==WM_RBUTTONDOWN)
		{
			pMsg->message=WM_LBUTTONDOWN;
		}
		else if(pMsg->message==WM_RBUTTONUP)
		{
			pMsg->message=WM_LBUTTONUP;
		}
	}
	return CallNextHookEx(hHook,idCode,wParam,lParam);
}

extern "C" __declspec(dllexport)
BOOL InstallHook()
{	
	// WH_GETMESSAGE - следит за сообщениями, появляющимися сразу после вызова функций GetMessage или PeekMessage.
	hHook=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc, hInstance, 0);
	return hHook!=NULL;
}

extern "C" __declspec(dllexport)
BOOL RemoveHook()
{
	return UnhookWindowsHookEx(hHook);
}