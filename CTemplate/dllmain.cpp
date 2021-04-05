#include "pch.h"

// BE AWARE ===v
// in order to reference other projects you need to add:
// $(SolutionDir)Common;%(AdditionalIncludeDirectories)
// to project properties -> c/c++ -> additional include directories
#include "ExampleHooks.h"
#include <ZXString.h>
#include <Common.h>
#include <hooker.h>
#include <memedit.h>

// BE AWARE ===v
// in order to fix the detours.lib link error you need to replace
// project properties -> vc++ directories -> library directories 
// with $(SolutionDir)Common;$(LibraryPath)

/// ================ \\\

// executed after the client is unpacked
VOID MainFunc()
{
	Log(__FUNCTION__);

	ZXString<char> pCharTest = ZXString<char>("test");
	pCharTest += " and another";

	Log(pCharTest);

	ZXString<wchar_t> pWCharTest = ZXString<wchar_t>(L"test");
	pWCharTest += L" and another";

	OutputDebugStringW(pWCharTest);

	return;

	// below hooks only serve as examples -- they will not do anything as-is

	INITMAPLEHOOK(
		_ExampleFunc_cdecl, // pointer to original function
		_ExampleFunc_cdecl_t, // function type
		MapleHooks::ExampleCDecl_Hook, // function to detour to
		0x0 // maple address
	);

	INITMAPLEHOOK(
		_ExampleFunc_thiscall, // pointer to original function
		_ExampleFunc_thiscall_t, // function type
		MapleHooks::ExampleFunc_thiscall2, // function to detour to
		0x0 // maple address
	);

	// edit memory

	WriteValue<BYTE>(0x0, 0xEB); // address to write to, value to write
	WriteValue<DWORD>(0x0, 0x42069);
	WriteValue<double>(0x0, 420.69);

	PatchNop(0x0, 6); // address to write to, number of nops

	PatchRetZero(0x0); // function start address to return zero at
}

// prolly don't edit this region if youre a noob
#pragma region EntryThread

// main thread
VOID MainProc()
{
	Log(__FUNCTION__);

	Common::CreateInstance
	(
		TRUE,			// true if you want to hook windows libraries (besides mutex) set this to false if you already edited your IP into the client (eg v83 localhosts)
		MainFunc,		// function to be executed after client is unpacked
		"127.0.0.1",	// IP to connect to (your server IP)
		"127.0.0.1"		// IP to redirect from (nexon IP)
	);
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
		Common::GetInstance()->~Common();
		break;
	}
	}
	return TRUE;
}

#pragma endregion