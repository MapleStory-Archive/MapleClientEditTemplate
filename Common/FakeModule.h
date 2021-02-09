#pragma once
#include <Windows.h>

class FakeModule
{
private:
	HMODULE hFakeModule;
	const char* szModuleName;

public:
	BOOL CreateModule(const char* szModuleName);
	BOOL DeleteModule();
};