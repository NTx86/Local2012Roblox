#pragma once
#include <iostream>
#include <string>
#include "Main.h"

struct HttpClass {
	int dummy[8];
	std::string url;
};


typedef void(__thiscall * _HttpGet)(HttpClass *AThis, void* a1, void* a2, void* a3, void* a4, std::string& a5);
extern _HttpGet HttpGet;

void __fastcall HttpGetHook(HttpClass *AThis, void* EDX, void* a1, void* a2, void* a3, void* a4, std::string& a5);