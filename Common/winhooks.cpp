#pragma once
#include "winhooks.h"
#include "winhook_types.h"
#include "hooker.h"
#include "logger.h"
#include "memedit.h"
#include "FakeModule.h"
#include "Common.h"

namespace WinHooks
{
	// fix returnaddress func
	// https://docs.microsoft.com/en-us/cpp/intrinsics/returnaddress?view=msvc-160
#pragma intrinsic(_ReturnAddress)

// link socket library
#pragma comment(lib, "WS2_32.lib")

// deprecated api call warning
#pragma warning(disable : 4996)

/// <summary>
/// Used to map out imports used by MapleStory.
/// The log output can be used to reconstruct the _ZAPIProcAddress struct
/// ZAPI struct is the dword before the while loop when searching for aob: 68 FE 00 00 00 ?? 8D
/// </summary>
	FARPROC WINAPI GetProcAddress_Hook(HMODULE hModule, LPCSTR lpProcName)
	{
		if (Common::GetInstance()->m_bThemidaUnpacked)
		{
			DWORD dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

			if (Common::GetInstance()->m_dwGetProcRetAddr != dwRetAddr)
			{
				Common::GetInstance()->m_dwGetProcRetAddr = dwRetAddr;

				Log("[GetProcAddress] Detected library loading from %08X.", dwRetAddr);
			}

			Log("[GetProcAddress] => %s", lpProcName);
		}

		return GetProcAddress_Original(hModule, lpProcName);
	}

	/// <summary>
	/// CreateMutexA is the first Windows library call after the executable unpacks itself.
	/// We hook this function to do all our memory edits and hooks when it's called.
	/// </summary>
	HANDLE WINAPI CreateMutexA_Hook(
		LPSECURITY_ATTRIBUTES lpMutexAttributes,
		BOOL				  bInitialOwner,
		LPCSTR				  lpName
	)
	{
		if (!CreateMutexA_Original)
		{
			Log("Original CreateMutex pointer corrupted. Failed to return mutex value to calling function.");

			return nullptr;
		}
		else if (lpName && strstr(lpName, Common::GetConfig()->MapleMutex))
		{
#if MAPLE_MULTICLIENT
			// from https://github.com/pokiuwu/AuthHook-v203.4/blob/AuthHook-v203.4/Client176/WinHook.cpp

			char szMutex[128];
			int nPID = GetCurrentProcessId();

			sprintf_s(szMutex, "%s-%d", lpName, nPID);
			lpName = szMutex;
#endif

#if !MAPLE_INSTAJECT
			Common::GetInstance()->OnThemidaUnpack();
#endif

			return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
		}

		return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
	}

	/// <summary>
	/// In some versions, Maple calls this library function to check if the anticheat has started.
	/// We can spoof this and return a fake handle for it to close.
	/// </summary>
	HANDLE WINAPI OpenMutexA_Hook(
		DWORD  dwDesiredAccess,
		BOOL   bInitialOwner,
		LPCSTR lpName
	)
	{
#if _DEBUG
		Log("Opening mutex %s", lpName);
#endif

		if (strstr(lpName, "meteora")) // make sure we only override hackshield
		{
			Log("Detected HS mutex => spoofing.");

			Common::GetInstance()->m_pFakeHsModule = new FakeModule();

			if (!Common::GetInstance()->m_pFakeHsModule->CreateModule("ehsvc.dll"))
			{
				Log("Unable to create fake HS module.");
			}
			else
			{
				Log("Fake HS module loaded.");
			}

			// return handle to a spoofed mutex so it can close the handle
			return CreateMutexA_Original(NULL, TRUE, "FakeMutex1");
		}
		else // TODO add second mutex handling
		{
			return OpenMutexA_Original(dwDesiredAccess, bInitialOwner, lpName);
		}
	}

	/// <summary>
	/// Used to track what maple is trying to start (mainly for anticheat modules).
	/// </summary>
	BOOL WINAPI CreateProcessW_Hook(
		LPCWSTR               lpApplicationName,
		LPWSTR                lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL                  bInheritHandles,
		DWORD                 dwCreationFlags,
		LPVOID                lpEnvironment,
		LPCWSTR               lpCurrentDirectory,
		LPSTARTUPINFOW        lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation
	)
	{
		if (Common::GetConfig()->HookToggleInfo.CreateProcess_Logging)
		{
			auto sAppName = lpApplicationName ? lpApplicationName : L"Null App Name";
			auto sArgs = lpCommandLine ? lpCommandLine : L"Null Args";

			Log("CreateProcessW -> %s : %s", sAppName, sArgs);
		}

		return CreateProcessW_Original(
			lpApplicationName, lpCommandLine, lpProcessAttributes,
			lpThreadAttributes, bInheritHandles, dwCreationFlags,
			lpEnvironment, lpCurrentDirectory, lpStartupInfo,
			lpProcessInformation
		);
	}

	/// <summary>
	/// Used same as above and also to kill/redirect some web requests.
	/// </summary>
	BOOL WINAPI CreateProcessA_Hook(
		LPCSTR                lpApplicationName,
		LPSTR                 lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL                  bInheritHandles,
		DWORD                 dwCreationFlags,
		LPVOID                lpEnvironment,
		LPCSTR                lpCurrentDirectory,
		LPSTARTUPINFOA        lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
#if MAPLETRACKING_CREATE_PROCESS
		auto sAppName = lpApplicationName ? lpApplicationName : "Null App Name";
		auto sArgs = lpCommandLine ? lpCommandLine : "Null Args";

		Log("CreateProcessA -> %s : %s", sAppName, sArgs);
#endif

		if (Common::GetConfig()->MapleExitWindowWebUrl && strstr(lpCommandLine, Common::GetConfig()->MapleExitWindowWebUrl))
		{
			Log("[CreateProcessA] [%08X] Killing web request to: %s", _ReturnAddress(), lpApplicationName);
			return FALSE; // ret value doesn't get used by maple after creating web requests as far as i can tell
		}

		return CreateProcessA_Original(
			lpApplicationName, lpCommandLine, lpProcessAttributes,
			lpThreadAttributes, bInheritHandles, dwCreationFlags,
			lpEnvironment, lpCurrentDirectory, lpStartupInfo,
			lpProcessInformation
		);
	}

	/// <summary>
	/// Same as CreateProcessW
	/// </summary>
	HANDLE WINAPI CreateThread_Hook(
		LPSECURITY_ATTRIBUTES   lpThreadAttributes,
		SIZE_T                  dwStackSize,
		LPTHREAD_START_ROUTINE  lpStartAddress,
		__drv_aliasesMem LPVOID lpParameter,
		DWORD                   dwCreationFlags,
		LPDWORD                 lpThreadId
	)
	{
		return CreateThread_Original(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	}

	/// <summary>
	/// Used to track what processes Maple opens.
	/// </summary>
	HANDLE WINAPI OpenProcess_Hook(
		DWORD dwDesiredAccess,
		BOOL  bInheritHandle,
		DWORD dwProcessId
	)
	{
		Log("OpenProcess -> PID: %d - CallAddy: %08X", dwProcessId, _ReturnAddress());

		return OpenProcess_Original(dwDesiredAccess, bInheritHandle, dwProcessId);
	}

	/// <summary>
	/// This library call is used by nexon to determine the locale of the connecting clients PC. We spoof it.
	/// </summary>
	/// <returns></returns>
	UINT WINAPI GetACP_Hook() // AOB: FF 15 ?? ?? ?? ?? 3D ?? ?? ?? 00 00 74 <- library call inside winmain func
	{
		UINT uiNewLocale = Common::GetConfig()->LocaleSpoofValue;

		if (!uiNewLocale) return GetACP_Original(); // should not happen cuz we dont hook if value is zero

		// we dont wanna unhook until after themida is unpacked
		// because if themida isn't unpacked then the call we are intercepting is not from maple
		if (Common::GetInstance()->m_bThemidaUnpacked)
		{
			DWORD dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

			// return address should be a cmp eax instruction because ret value is stored in eax
			// and nothing else should happen before the cmp
			if (ReadValue<BYTE>(dwRetAddr) == x86CMPEAX)
			{
				uiNewLocale = ReadValue<DWORD>(dwRetAddr + 1); // check value is always 4 bytes

				Log("[GetACP] Found desired locale: %d", uiNewLocale);
			}
			else
			{
				Log("[GetACP] Unable to automatically determine locale, using stored locale: %d", uiNewLocale);
			}

			Log("[GetACP] Locale spoofed to %d, unhooking. Calling address: %08X", uiNewLocale, dwRetAddr);

			if (!SetHook(FALSE, reinterpret_cast<void**>(&GetACP_Original), GetACP_Hook))
			{
				Log("Failed to unhook GetACP.");
			}
		}

		return uiNewLocale;
	}

	/// <summary>
	/// Blocks the startup patcher "Play!" window and forces the login screen to be minimized
	/// </summary>
	HWND WINAPI CreateWindowExA_Hook(
		DWORD     dwExStyle,
		LPCSTR    lpClassName,
		LPCSTR    lpWindowName,
		DWORD     dwStyle,
		int       X,
		int       Y,
		int       nWidth,
		int       nHeight,
		HWND      hWndParent,
		HMENU     hMenu,
		HINSTANCE hInstance,
		LPVOID    lpParam
	)
	{
		Log("[CreateWindowExA] => %s - %s", lpClassName, lpWindowName);

		if (Common::GetConfig()->MaplePatcherClass && strstr(lpClassName, Common::GetConfig()->MaplePatcherClass))
		{
			Log("Bypassing patcher window..");

			if (Common::GetConfig()->InjectImmediately)
			{
				Common::GetInstance()->OnThemidaUnpack();
			}

			return NULL;
		}
		else
		{
			if (Common::GetConfig()->ForceWindowedOnStart
				&& Common::GetConfig()->MapleWindowClass
				&& strstr(lpClassName, Common::GetConfig()->MapleWindowClass))
			{
				dwExStyle = 0;
				dwStyle = 0xCA0000;
			}

			if (Common::GetConfig()->InjectImmediately)
			{
				Common::GetInstance()->OnThemidaUnpack();
			}

			return CreateWindowExA_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		}
	}

	/// <summary>
	/// We use this function to track what memory addresses are killing the process.
	/// There are more ways that Maple kills itself, but this is one of them.
	/// </summary>
	LONG NTAPI NtTerminateProcess_Hook(
		HANDLE hProcHandle,
		LONG   ntExitStatus
	)
	{
		Log("NtTerminateProcess: %08X", unsigned(_ReturnAddress()));

		return NtTerminateProcess_Original(hProcHandle, ntExitStatus);
	}

	/// <summary>
	/// Maplestory saves registry information (config stuff) for a number of things. This can be used to track that.
	/// </summary>
	LSTATUS WINAPI RegCreateKeyExA_Hook(
		HKEY                        hKey,
		LPCSTR                      lpSubKey,
		DWORD                       Reserved,
		LPSTR                       lpClass,
		DWORD                       dwOptions,
		REGSAM                      samDesired,
		const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		PHKEY                       phkResult,
		LPDWORD                     lpdwDisposition
	)
	{
		Log("RegCreateKeyExA - Return address: %d", _ReturnAddress());

		return RegCreateKeyExA_Original(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
	}

	namespace WinSock
	{

		/// <summary>
		/// 
		/// </summary>
		INT WSPAPI WSPConnect_Hook(
			SOCKET				   s,
			const struct sockaddr* name,
			int					   namelen,
			LPWSABUF			   lpCallerData,
			LPWSABUF			   lpCalleeData,
			LPQOS				   lpSQOS,
			LPQOS				   lpGQOS,
			LPINT				   lpErrno
		)
		{
			char szAddr[50];
			DWORD dwLen = 50;
			WSAAddressToString((sockaddr*)name, namelen, NULL, szAddr, &dwLen);

			sockaddr_in* service = (sockaddr_in*)name;

#if MAPLETRACKING_WSPCONN_PRINT
			Log("WSPConnect IP Detected: %s", szAddr);
#endif

			if (strstr(szAddr, Common::GetInstance()->m_sOriginalIP))
			{
				Log("Detected and rerouting socket connection to IP: %s", Common::GetInstance()->m_sRedirectIP);
				service->sin_addr.S_un.S_addr = inet_addr(Common::GetInstance()->m_sRedirectIP);
				Common::GetInstance()->m_GameSock = s;
			}

			return Common::GetInstance()->m_ProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
		}

		/// <summary>
		/// 
		/// </summary>
		INT WSPAPI WSPGetPeerName_Hook(
			SOCKET			 s,
			struct sockaddr* name,
			LPINT			 namelen,
			LPINT			 lpErrno
		)
		{
			int nRet = Common::GetInstance()->m_ProcTable.lpWSPGetPeerName(s, name, namelen, lpErrno);

			if (nRet != SOCKET_ERROR)
			{
				char szAddr[50];
				DWORD dwLen = 50;
				WSAAddressToString((sockaddr*)name, *namelen, NULL, szAddr, &dwLen);

				sockaddr_in* service = (sockaddr_in*)name;

				USHORT nPort = ntohs(service->sin_port);

				if (s == Common::GetInstance()->m_GameSock)
				{
					char szAddr[50];
					DWORD dwLen = 50;
					WSAAddressToString((sockaddr*)name, *namelen, NULL, szAddr, &dwLen);

					sockaddr_in* service = (sockaddr_in*)name;

					u_short nPort = ntohs(service->sin_port);

					service->sin_addr.S_un.S_addr = inet_addr(Common::GetInstance()->m_sRedirectIP);

					Log("WSPGetPeerName => IP Replaced: %s -> %s", szAddr, Common::GetInstance()->m_sOriginalIP);
				}
				else
				{
					Log("WSPGetPeerName => IP Ignored: %s:%d", szAddr, nPort);
				}
			}
			else
			{
				Log("WSPGetPeerName Socket Error: %d", nRet);
			}

			return nRet;
		}

		/// <summary>
		/// 
		/// </summary>
		INT WSPAPI WSPCloseSocket_Hook(
			SOCKET s,
			LPINT  lpErrno
		)
		{
			int nRet = Common::GetInstance()->m_ProcTable.lpWSPCloseSocket(s, lpErrno);

			if (s == Common::GetInstance()->m_GameSock)
			{
				Log("Socket closed by application.. (%d). CallAddr: %02x", nRet, _ReturnAddress());
				Common::GetInstance()->m_GameSock = INVALID_SOCKET;
			}

			return nRet;
		}

		/// <summary>
		/// 
		/// </summary>
		INT WSPAPI WSPStartup_Hook(
			WORD				wVersionRequested,
			LPWSPDATA			lpWSPData,
			LPWSAPROTOCOL_INFOW lpProtocolInfo,
			WSPUPCALLTABLE		UpcallTable,
			LPWSPPROC_TABLE		lpProcTable
		)
		{
			int nRet = WSPStartup_Original(wVersionRequested, lpWSPData, lpProtocolInfo, UpcallTable, lpProcTable);

			if (nRet == NO_ERROR)
			{
				Log("Overriding socket routines..");

				Common::GetInstance()->m_GameSock = INVALID_SOCKET;
				Common::GetInstance()->m_ProcTable = *lpProcTable;

				lpProcTable->lpWSPConnect = WSPConnect_Hook;
				lpProcTable->lpWSPGetPeerName = WSPGetPeerName_Hook;
				lpProcTable->lpWSPCloseSocket = WSPCloseSocket_Hook;
			}
			else
			{
				Log("WSPStartup Error Code: %d", nRet);
			}

			return nRet;
		}
	}
}