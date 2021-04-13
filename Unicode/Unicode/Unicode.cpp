#define _UNICODE 
#define UNICODE

#include <iostream>
#include <Windows.h>
#include <tchar.h>

using namespace std;

void UnicodeType();
void TcharTest();
void WindowsFunctions();
void ConvertAnsiToUnicodeCpp();
void ConvertUnicodeToAnsiCpp();
void ConvertAnsiToUnicodeWinApi();
void ConvertUnicodeToAnsiWinApi();

int main()
{
	//UnicodeType();
	//TcharTest();
	//WindowsFunctions();
	//ConvertAnsiToUnicodeCpp();
	//ConvertUnicodeToAnsiCpp();
	//ConvertAnsiToUnicodeWinApi();
	ConvertUnicodeToAnsiWinApi();
}

void UnicodeType()
{
	// ANSI-��������� 1 symbol - 1 byte
	char szBuf1[15] = "Hello,";
	strcat_s(szBuf1, " world!");
	cout << "String: " << szBuf1 << endl;
	cout << sizeof(szBuf1) << " bytes\n"; // 15 ����

	// UNICODE-��������� 1 symbol - 2 byte
	wchar_t szBuf2[15] = L"Hello,";
	wcscat_s(szBuf2, L" world!");
	wcout << "String: " << szBuf2 << endl;
	cout << sizeof(szBuf2) << " bytes\n"; // 30 ����
}

void TcharTest()
{
	TCHAR szBuf3[15] = TEXT("Hello,");
	_tcscat_s(szBuf3, 15, TEXT(" world!"));
	wcout << szBuf3 << '\n';
	cout << "The size of array: " << sizeof(szBuf3) << " bytes\n"; // 30 ����
}

void WindowsFunctions()
{
	char szBuf1[15] = "Hello,";
	lstrcatA(szBuf1, " world!");
	cout << "String: " << szBuf1 << endl;

	wchar_t szBuf2[15] = L"Hello,";
	lstrcatW(szBuf2, L" world!");
	wcout << "String: " << szBuf2 << endl;

	TCHAR szBuf3[15] = TEXT("Hello, ");
	lstrcat(szBuf3, TEXT("world!"));
	wcout << "String: " << szBuf3 << endl;
}

void ConvertAnsiToUnicodeCpp()
{
	char buffer[] = "mbstowcs converts ANSI-string to Unicode-string";

	// ��������� ������ ������, ����������� ��� �������� Unicode-������
	size_t length;
	mbstowcs_s(&length, nullptr, 0, buffer, 0);
	
	wchar_t* ptr = new wchar_t[length];

	//  ������������ ANSI-������ � Unicode-������
	mbstowcs_s(&length, ptr, length, buffer, length);
	

	wcout << ptr;
	cout << "\nLength of Unicode-string: " << length << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << " bytes" << endl;
	
	delete[] ptr;
}

void ConvertUnicodeToAnsiCpp()
{
	wchar_t buffer[] = L"wcstombs converts Unicode-string to ANSI-string";
	
	// ��������� ������ ������, ����������� ��� �������� ��������������� ANSI-������
	size_t length;
	wcstombs_s(&length, nullptr, 0, buffer, 0);
	
	char* ptr = new char[length];
	
	// ������������ Unicode-������ � ANSI-������ 
	wcstombs_s(nullptr, ptr, length, buffer, length);
	
	cout << ptr;
	cout << "\nLength of ANSI-string: " << strlen(ptr) << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << " bytes" << endl;
	delete[] ptr;
}

void ConvertAnsiToUnicodeWinApi()
{
	char buffer[] = "MultiByteToWideChar converts ANSI-string to Unicode-string";
	
	// ��������� ������ ������, ����������� ��� �������� Unicode-������
	int length = MultiByteToWideChar(CP_ACP /*ANSI code page*/, 0, buffer, -1, NULL, 0);
	
	wchar_t* ptr = new wchar_t[length];
	
	// ������������ ANSI-������ � Unicode-������
	MultiByteToWideChar(CP_ACP, 0, buffer, -1, ptr, length);
	
	wcout << ptr << endl;
	cout << "Length of Unicode-string: " << wcslen(ptr) << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << endl;
	delete[] ptr;
}

void ConvertUnicodeToAnsiWinApi()
{
	wchar_t buffer[] = L"WideCharToMultiByte converts Unicode-string to ANSI-string";
	
	// ��������� ������ ������, ����������� ��� �������� ANSI-������
	int length = WideCharToMultiByte(CP_ACP /*ANSI code page*/, 0, buffer, -1, NULL, 0, 0, 0);
	
	char* ptr = new char[length];

	// ������������ Unicode-������ � ANSI-������
	char defaultSymbol = '?';
	int isDefSymbol = 0;

	WideCharToMultiByte(CP_ACP, 0, buffer, -1, ptr, length, &defaultSymbol, &isDefSymbol);

	cout << ptr << endl;
	cout << "Length of ANSI-string: " << strlen(ptr) << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << endl;
	cout << "Used default symbol: " << isDefSymbol << endl;

	delete[] ptr;
}
