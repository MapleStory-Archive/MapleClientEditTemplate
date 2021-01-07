#pragma once

// declare your hook functions

namespace MapleHooks
{
	void __cdecl ExampleCDecl_Hook(void* pArg1, int nArg2);
	int __fastcall ExampleFunc_thiscall2(void* pThis, void* edx, int nArg1);
}