#pragma comment( lib, "FirstDll" )

#include <iostream>
using namespace std;

#include "../FirstDll/FirstDll.h"

int main()
{
    cout << "fooInDll: " << fooInDll() << endl;
    cout << "barInDll: " << barInDll() << endl;
}
