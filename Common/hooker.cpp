#include "hooker.h"

BOOL SetHook(bool bInstall, void** ppvTarget, void* pvDetour)
{
	if (DetourTransactionBegin() != NO_ERROR)
	{
		return FALSE;
	}

	HANDLE pCurThread = GetCurrentThread();

	if (DetourUpdateThread(pCurThread) == NO_ERROR)
	{
		auto pDetourFunc = bInstall ? DetourAttach : DetourDetach;

		if (pDetourFunc(ppvTarget, pvDetour) == NO_ERROR)
		{
			if (DetourTransactionCommit() == NO_ERROR)
			{
				return TRUE;
			}
		}
	}

	DetourTransactionAbort();
	return FALSE;
}

DWORD GetFuncAddress(const char* lpModule, const char* lpFunc)
{
	HMODULE hMod = LoadLibraryA(lpModule);

	return !hMod ? 0 : (DWORD)GetProcAddress(hMod, lpFunc);
}
