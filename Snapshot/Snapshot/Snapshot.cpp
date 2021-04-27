#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <tlhelp32.h>

using namespace std;

void PrintProcessInfo(PROCESSENTRY32& processInfo);
void PrintProcessHeader();
wstring PriorityToString(LONG priority);

void PrintError();

int main()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == NULL)
    {
        PrintError();
        return 0;
    }

    PROCESSENTRY32 processInfo = { sizeof(PROCESSENTRY32) };

    BOOL res = Process32First(hSnapshot, &processInfo);

    if (res == TRUE) {

        PrintProcessHeader();

        do {

            PrintProcessInfo(processInfo);

        } while (Process32Next(hSnapshot, &processInfo));
    }

    CloseHandle(hSnapshot);

    return 0;
}

void PrintProcessInfo(PROCESSENTRY32& processInfo)
{
    wcout << setw(40) << left << processInfo.szExeFile;
    wcout << setw(10) << processInfo.th32ProcessID;
    wcout << setw(10) << processInfo.cntThreads;
    wcout << setw(20) << PriorityToString(processInfo.pcPriClassBase);
    wcout << endl;

}

void PrintProcessHeader()
{
    wcout << setw(40) << left << L"Name";
    wcout << setw(10) << L"Id";
    wcout << setw(10) << L"Threads";
    wcout << setw(20) << L"Priority";
    wcout << endl;
    wcout << setw(60) << setfill(L'=') << L"=" << endl;
    wcout << setfill(L' ');
}

wstring PriorityToString(LONG priority)
{
    wstring str;

    switch (priority)
    {
    case 8:
        str = L"Normal";
        break;
    case 10:
        str = L"Above normal";
        break;
    case  6:
        str = L"Below normal";
        break;
    case 13:
        str = L"High";
        break;
    case 4:
        str = L"Idle";
        break;
    case 24:
        str = L"Realtime";
        break;   
    default:
        str = to_wstring(priority);
        break;
    }

    return str;
}

void PrintError()
{
    int errorCode = GetLastError();

    WCHAR errorMessage[500];
    LPVOID lpBuffer = NULL;

    DWORD res = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpBuffer,
        0,
        NULL
    );

    if (res != 0)
    {
        wsprintf(errorMessage, L"Error: %s", lpBuffer);
        wcout << errorMessage << endl;
    }
    else {
        wcout << L"Error: " << endl;
    }


}
