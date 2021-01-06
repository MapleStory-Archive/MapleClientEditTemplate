#include "pch.h"

// BE AWARE ===v
// in order to reference other projects you need to add:
// $(SolutionDir)Common;%(AdditionalIncludeDirectories)
// to project properties -> c/c++ -> additional include directories
#include "memedit.h"
#include "hooker.h"
#include "Common.h"
#include "winhooks.h"

// BE AWARE ===v
// in order to fix the detours.lib link error you need to replace
// project properties -> vc++ directories -> library directories 
// with $(SolutionDir)Common;$(LibraryPath)

/// ================ \\\

// executed after the client is unpacked
VOID MainFunc()
{
	Log(__FUNCTION__);

	// paste all your edits in here
}

// prolly don't edit this region if youre a noob
#pragma region EntryThread

// windows hooks are stored in here -> no need to mess with this normally
static Common* CommonHooks;

// main thread
VOID MainProc()
{
	Log(__FUNCTION__);

	if (MAPLE_INSTAJECT)
	{
		MainFunc();
	}
	else
	{
		CommonHooks = new Common(
			TRUE,			// true if you want to hook windows libraries (besides mutex)
							// set this to false if you already edited your IP into the client (eg v83 localhosts)
			&MainFunc,		// function to be executed after client is unpacked
			"127.0.0.1",	// IP to connec to (your server IP)
			"127.0.0.1");	// IP to redirect from (nexon IP)
	}
}

// dll entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		Log("DLL_PROCESS_ATTACH");

		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainProc, NULL, 0, 0);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		Log("DLL_PROCESS_DETACH");
		CommonHooks->~Common();
		break;
	}
	}
	return TRUE;
}

#pragma endregion