#pragma once
#include <Windows.h>
#include <stdlib.h>

// thanks raj for some of these

#define relative_address(frm, to) (int)(((int)to - (int)frm) - 5)

#define x86CMPEAX 0x3D
#define x86XOR 0x33
#define x86EAXEAX 0xC0
#define x86RET 0xC3
#define x86JMP 0xE9
#define x86CALL 0xE8
#define x86NOP 0x90

static VOID PatchRetZero(DWORD dwAddress)
{
	*(BYTE*)(dwAddress + 0) = x86XOR;
	*(BYTE*)(dwAddress + 1) = x86EAXEAX;
	*(BYTE*)(dwAddress + 2) = x86RET;
}

static VOID PatchJmp(DWORD dwAddress, PVOID pDestination)
{
	*(BYTE*)(dwAddress + 0) = x86JMP;
	*(DWORD*)(dwAddress + 1) = relative_address(dwAddress, pDestination);
}

static VOID PatchCall(DWORD dwAddress, PVOID pDestination)
{
	*(BYTE*)(dwAddress + 0) = x86CALL;
	*(DWORD*)(dwAddress + 1) = relative_address(dwAddress, pDestination);
}

static VOID PatchNop(DWORD dwAddress, UINT nCount)
{
	for (int i = 0; i < nCount; i++)
		*(BYTE*)(dwAddress + i) = x86NOP;
}

template <typename T>
static VOID WriteValue(DWORD dwAddress, T pValue)
{
	*((T*)dwAddress) = pValue;
}

static VOID WriteBytes(DWORD dwAddress, const char* pData, UINT nCount)
{
	memcpy((PVOID)dwAddress, pData, nCount);
}

template <typename T>
static T ReadValue(DWORD dwAddr)
{
	return *((T*)dwAddr);
}