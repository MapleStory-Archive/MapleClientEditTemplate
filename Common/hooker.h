#pragma once
#include <Windows.h>
#include "detours.h"

#pragma comment(lib, "detours.lib")

#pragma region Macros

/// Sets hook and outputs result to debug window (pass/fail)
#define INITWINHOOK(sModName, sFuncName, pOrigFunc, Func_t, pNewFunc)		\
pOrigFunc = (Func_t)GetFuncAddress(sModName, sFuncName);					\
if (SetHook(TRUE, reinterpret_cast<void**>(&pOrigFunc), pNewFunc))			\
{ Log("Hooked %s", sFuncName); }											\
else																		\
{ Log("Failed to hook %s", sFuncName); } // end macro

#define INITMAPLEHOOK(pOrigFunc, Func_t, pNewFunc, dwAddress)				\
pOrigFunc = reinterpret_cast<Func_t>(dwAddress);							\
if (!SetHook(TRUE, reinterpret_cast<void**>(&pOrigFunc), pNewFunc))			\
{ Log("Failed to hook maple func at address %d", dwAddress); } // end macro

#define HOOKDEF(retType, callConv, defName, typeName, funcName, ...)		\
typedef retType (callConv* typeName)(__VA_ARGS__);							\
typeName defName = nullptr;													\
retType callConv funcName(__VA_ARGS__);

#pragma endregion

extern BOOL SetHook(bool bInstall, void** ppvTarget, void* pvDetour);
extern DWORD GetFuncAddress(const char* lpModule, const char* lpFunc);
extern PVOID HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset);