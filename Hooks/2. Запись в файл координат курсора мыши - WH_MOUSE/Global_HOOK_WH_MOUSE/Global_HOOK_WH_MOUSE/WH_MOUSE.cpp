#define __MYDLL__
#include "WH_HOOK.h"
#include "stdio.h"

HHOOK hHook; 
HINSTANCE hMod;


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
bool installhook ()
{
	return hHook = SetWindowsHookEx(WH_MOUSE, MouseProc, hMod, 0);
}
bool removehook ()
{
	return UnhookWindowsHookEx(hHook);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == HC_ACTION)&&(wParam == WM_MOUSEMOVE))
	{
		MOUSEHOOKSTRUCT * mouse =  (MOUSEHOOKSTRUCT *)lParam;
		FILE * pFile;
		pFile = fopen("c:\\test\\hook.txt", "a");
		if (pFile)
		{
			char str [300];
			char temp [300];
			/*itoa(mouse->pt.x, str, 10);
			strcpy (temp, str);
			strcat(temp, "   ");
			itoa(mouse->pt.y, str, 10);
			strcat (temp, str);*/
			sprintf_s(temp, "x: %d\ty: %d", mouse->pt.x, mouse->pt.y);

			fputs(temp, pFile);
			fputs("\n", pFile);
			fclose(pFile);
		}
		return 0;
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hMod = (HINSTANCE) hModule;
	}
	return TRUE;
}
