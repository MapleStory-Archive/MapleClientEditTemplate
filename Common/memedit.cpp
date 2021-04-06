#include "memedit.h"

VOID PatchRetZero(DWORD dwAddress)
{
	*(BYTE*)(dwAddress + 0) = x86XOR;
	*(BYTE*)(dwAddress + 1) = x86EAXEAX;
	*(BYTE*)(dwAddress + 2) = x86RET;
}

VOID PatchJmp(DWORD dwAddress, PVOID pDestination)
{
	*(BYTE*)(dwAddress + 0) = x86JMP;
	*(DWORD*)(dwAddress + 1) = relative_address(dwAddress, pDestination);
}

VOID PatchCall(DWORD dwAddress, PVOID pDestination)
{
	*(BYTE*)(dwAddress + 0) = x86CALL;
	*(DWORD*)(dwAddress + 1) = relative_address(dwAddress, pDestination);
}

VOID PatchNop(DWORD dwAddress, UINT nCount)
{
	for (UINT i = 0; i < nCount; i++)
		*(BYTE*)(dwAddress + i) = x86NOP;
}

VOID WriteBytes(DWORD dwAddress, const char* pData, UINT nCount)
{
	memcpy((PVOID)dwAddress, pData, nCount);
}