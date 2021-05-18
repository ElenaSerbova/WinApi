#pragma once

#ifndef FIRSTDLL_API
	#define FIRSTDLL_API extern "C" __declspec(dllimport)
#endif

FIRSTDLL_API int fooInDll();
FIRSTDLL_API int barInDll();
FIRSTDLL_API const double Pi;