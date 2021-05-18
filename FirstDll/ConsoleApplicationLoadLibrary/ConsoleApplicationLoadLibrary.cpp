#include <Windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

typedef int (*FOO_IN_DLL)();

int main()
{
    HINSTANCE hInstLib = LoadLibrary(TEXT("FirstDll.dll"));

    if (hInstLib == NULL)
    {
        cout << "Can`t load library FirstDll.dll" << endl;
        return 0;
    }

    FOO_IN_DLL fooInDll = (FOO_IN_DLL)GetProcAddress(hInstLib, "fooInDll");

    if (fooInDll == NULL)
    {
        cout << "Can`t load function fooInDll" << endl;
    }
    else
    {
        cout << "fooInDll: " << fooInDll() << endl;
    }

    const double* pPi = (const double*)GetProcAddress(hInstLib, "Pi");
    cout << "Pi: " << *pPi << endl;

    FreeLibrary(hInstLib);
}
