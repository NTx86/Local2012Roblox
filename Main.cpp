// HeadlessRoblox.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "Main.h"
#include "RBXHTTP.h"
#include "HookUtil.h"

_RBXPrint RBXPrint = (_RBXPrint)0x006B97B0;
_RBXPrintObj RBXPrintObj = (_RBXPrintObj)0x006B9AE0;

extern "C" __declspec(dllexport) DWORD WINAPI MainThread(LPVOID param)
{
	HttpGet = (_HttpGet)HookTramp((void*)0x008624B0, HttpGetHook, 5); 
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	default:
		break;
	}
	return TRUE;
}

