#define FIRSTDLL_API extern "C" __declspec(dllexport)

#include "FirstDll.h"

FIRSTDLL_API const int Pi = 3.1415;


FIRSTDLL_API int fooInDll()
{
	int a = 100;
	return a;
}

FIRSTDLL_API int barInDll()
{
	return 0;
}