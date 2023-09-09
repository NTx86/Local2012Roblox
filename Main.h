#pragma once
#include "Windows.h"

extern "C" __declspec(dllexport) DWORD WINAPI MainThread(LPVOID param);

typedef void(__cdecl * _RBXPrint)(void* arg0, int color, const char *Format, ...);
extern _RBXPrint RBXPrint;

typedef void*(__cdecl * _RBXPrintObj)(void* Obj);
extern _RBXPrintObj RBXPrintObj;