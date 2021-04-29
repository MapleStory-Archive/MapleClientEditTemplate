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

extern VOID PatchRetZero(DWORD dwAddress);
extern VOID PatchJmp(DWORD dwAddress, PVOID pDestination);
extern VOID PatchCall(DWORD dwAddress, PVOID pDestination);
extern VOID PatchNop(DWORD dwAddress, UINT nCount);
extern VOID WriteBytes(DWORD dwAddress, const char* pData, UINT nCount);

template <typename TType>
VOID WriteValue(DWORD dwAddress, TType pValue)
{
	*((TType*)dwAddress) = pValue;
}

template <typename TType>
TType ReadValue(DWORD dwAddr)
{
	return *((TType*)dwAddr);
}