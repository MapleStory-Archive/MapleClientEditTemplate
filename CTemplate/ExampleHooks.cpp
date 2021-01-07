#include "pch.h"
#include "ExampleHooks.h"

// define your hook functions

namespace MapleHooks
{
	/// <summary>
	/// Example regular cdecl function hook
	/// </summary>
	/// <param name="pArg1">First arg</param>
	/// <param name="nArg2">second arg</param>
	/// <returns>Nothing</returns>
	void __cdecl ExampleCDecl_Hook(void* pArg1, int nArg2)
	{
		Log("Detour triggered!");

		// we only want to execute this function if nArg2 is more than 420, otherwise we return without doing anything
		if (nArg2 > 420)
		{
			_ExampleFunc_cdecl(pArg1, nArg2);
		}
	}

	/// <summary>
	/// Example member function call hook
	/// </summary>
	/// <param name="pThis">Member object</param>4
	/// <param name="edx">Dummy parameter because we are swapping calling conventions.</param>
	/// <param name="nArg1">Some value</param>
	/// <returns>An integer</returns>
	int __fastcall ExampleFunc_thiscall2(void* pThis, void* edx, int nArg1)
	{
		Log("Detour func called, arg1: %d", nArg1);
		
		if (nArg1 == 420)
		{
			nArg1 = 69; // possible to change args or do whatever

			Log("Arg changed to 69");
		}

		return _ExampleFunc_thiscall(pThis, NULL, nArg1); // second argument should be null because of the __thiscall -> __fastcall conversion
	}
}