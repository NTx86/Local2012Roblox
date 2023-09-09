#pragma once

bool HookFunc(void* HookAddress, void* Func);
void* HookTramp(void* HookAddress, void* Func, int len);