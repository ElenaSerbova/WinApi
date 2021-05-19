#pragma once
#include <windows.h>
#ifdef  __MYDLL__
#define __HOOK__ extern "C" __declspec (dllexport)
#else
#define __HOOK__ extern "C" __declspec (dllimport)
#endif

__HOOK__ bool installhook ();
__HOOK__ bool removehook ();
