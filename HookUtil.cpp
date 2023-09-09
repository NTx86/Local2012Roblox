#include <windows.h>

bool HookFunc(void* HookAddress, void* Func)
{
	DWORD Protection;
	DWORD Temp;

	VirtualProtect((LPVOID)HookAddress, 5, PAGE_EXECUTE_READWRITE, &Protection);
	uintptr_t RelativeAddress = ((uintptr_t)Func - (uintptr_t)HookAddress) - 5;
	*(unsigned char*)HookAddress = 0xE9;
	*(uintptr_t*)((uintptr_t)HookAddress+1) = RelativeAddress;
	VirtualProtect((LPVOID)HookAddress, 5, Protection, &Temp);
	return true;
}

void* HookTramp(void* HookAddress, void* Func, int len)
{
	uintptr_t RelativeAddress;
	void* SavedOpcodes = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	//copy over the code we overwrote
	memcpy(SavedOpcodes, HookAddress, len);
	//make the jmp back to actual function
	RelativeAddress = ((uintptr_t)HookAddress - ((uintptr_t)SavedOpcodes) - 5);
	*(unsigned char*)((uintptr_t)SavedOpcodes+len) = 0xE9;
	*(uintptr_t*)((uintptr_t)SavedOpcodes+len+1) = RelativeAddress;
	//do the hook
	HookFunc(HookAddress, Func);
	return SavedOpcodes;
}