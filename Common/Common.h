#pragma once
#include <Windows.h>
#include "hooker.h"
#include "logger.h"
#include "winhooks.h"

struct Common
{
public:
	Common(BOOL bHookWinLibs, PostMutexFunc_t pMutexFunc, const char* sIP, const char* sOriginalIP)
	{
		Log("Common created => Hook winsock libs: %s || IP: %s || Original IP: %s", (bHookWinLibs ? "Yes" : "No"), sIP, sOriginalIP);

		if (!pMutexFunc)
		{
			Log("Invalid function pointer passed to Common constructor.");
			return;
		}

		// set pointer to function that is executed after client unpacks itself
		g_PostMutexFunc = pMutexFunc;

		// required for proper injection
		INITWINHOOK("KERNEL32", "CreateMutexA", CreateMutexA_Original, CreateMutexA_t, CreateMutexA_Hook);

		if (MAPLE_PATCHER_CLASS)
		{
			INITWINHOOK("USER32", "CreateWindowExA", CreateWindowExA_Original, CreateWindowExA_t, CreateWindowExA_Hook);
		}

		if (MAPLE_LOCALE_SPOOF)
		{
			INITWINHOOK("KERNEL32", "GetACP", GetACP_Original, GetACP_t, GetACP_Hook);
		}

#if MAPLETRACKING_OPEN_PROC
		INITWINHOOK("KERNEL32", "OpenProcess", OpenProcess_Original, OpenProcess_t, OpenProcess_Hook);
#endif

#if MAPLETRACKING_CREATE_PROCESS
		INITWINHOOK("KERNEL32", "CreateProcessW", CreateProcessW_Original, CreateProcessW_t, CreateProcessW_Hook);
		INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, CreateProcessA_Hook);

#else
		if (MAPLE_KILL_EXIT_WINDOW)
		{
			INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, CreateProcessA_Hook);
		}
#endif

#if MAPLETRACKING_OPEN_MUTEXA
		INITWINHOOK("KERNEL32", "OpenMutexA", OpenMutexA_Original, OpenMutexA_t, OpenMutexA_Hook);
#endif

#if MAPLETRACKING_NT_TERMINATE_PROC
		INITWINHOOK("NTDLL", "NtTerminateProcess", NtTerminateProcess_Original, NtTerminateProcess_t, NtTerminateProcess_Hook);
#endif

#if MAPLETRACKING_REGCREATEKEY
		INITWINHOOK("KERNEL32", "RegCreateKeyExA", RegCreateKeyExA_Original, RegCreateKeyExA_t, RegCreateKeyExA_Hook);
#endif

#if MAPLETRACKING_GETPROCADDR
		INITWINHOOK("KERNEL32", "GetProcAddress", GetProcAddress_Original, GetProcAddress_t, GetProcAddress_Hook);
#endif

		if (!bHookWinLibs) return;

		if (!sIP || !sOriginalIP)
		{
			Log("Null IP string passed to Common constructor.");
			return;
		}

		g_sRedirectIP = sIP;
		g_sOriginalIP = sOriginalIP;

		// macro outputs error if hook fails
		INITWINHOOK("MSWSOCK", "WSPStartup", WSPStartup_Original, WSPStartup_t, WSPStartup_Hook);
	}

	~Common()
	{
		Log("Cleaning up common..");

		if (g_GameSock != INVALID_SOCKET)
		{
			Log("Closing socket..");

			g_ProcTable.lpWSPCloseSocket(g_GameSock, nullptr);
			g_GameSock = INVALID_SOCKET;
		}
	}
};

