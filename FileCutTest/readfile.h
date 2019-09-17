#pragma once
#include "pch.h"
#include <string>
#include <iostream>

using namespace std;

#ifdef UNICODE
#define fstring std::wstring
#else
#define fstring std::string
#endif // !UNICODE

bool FileEncryption(fstring fsSrcPath, fstring fsDesPath);

char MakecodeChar(char c, int key);
char CutcodeChar(char c, int key);
void Makecode(char *pstr, int *pkey);
void Cutecode(char *pstr, int *pkey);
//void foo(int i, DWORD dLen, LPVOID pFileBlock, bool en);
