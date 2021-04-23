#include "Common.h"

Common* Common::_s_pInstance;
Common::Config* Common::_s_pConfig;

Common::Common(BOOL bHookWinLibs, std::function<void()> pPostMutexFunc, const char* sIP, const char* sOriginalIP)
{
	this->m_sRedirectIP = sIP;
	this->m_sOriginalIP = sOriginalIP;
	this->m_GameSock = INVALID_SOCKET;
	this->m_ProcTable = { 0 };
	this->m_bThemidaUnpacked = FALSE;
	this->m_dwGetProcRetAddr = 0;

	if (!pPostMutexFunc)
	{
#if _DEBUG
		Log("Invalid function pointer passed to Common constructor.");
#endif
		return;
	}

	if (this->GetConfig()->InjectImmediately) // call post-unpack function right away
	{
		pPostMutexFunc();
	}
	else // set pointer to function that is executed after client unpacks itself
	{
		this->m_PostMutexFunc = pPostMutexFunc;
	}

#if _DEBUG
	Log("Common created => Hook winsock libs: %s || IP: %s || Original IP: %s", (bHookWinLibs ? "Yes" : "No"), sIP, sOriginalIP);
#endif

	// required for proper injection
	INITWINHOOK("KERNEL32", "CreateMutexA", CreateMutexA_Original, CreateMutexA_t, WinHooks::CreateMutexA_Hook);

	if (Common::GetConfig()->MaplePatcherClass || Common::GetConfig()->MapleWindowClass || Common::GetConfig()->InjectImmediately)
	{
		INITWINHOOK("USER32", "CreateWindowExA", CreateWindowExA_Original, CreateWindowExA_t, WinHooks::CreateWindowExA_Hook);
	}

	if (Common::GetConfig()->LocaleSpoofValue)
	{
		INITWINHOOK("KERNEL32", "GetACP", GetACP_Original, GetACP_t, WinHooks::GetACP_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.OpenProcess_Logging)
	{
		INITWINHOOK("KERNEL32", "OpenProcess", OpenProcess_Original, OpenProcess_t, WinHooks::OpenProcess_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.CreateProcess_Logging)
	{
		INITWINHOOK("KERNEL32", "CreateProcessW", CreateProcessW_Original, CreateProcessW_t, WinHooks::CreateProcessW_Hook);
		INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, WinHooks::CreateProcessA_Hook);
	}
	else if (Common::GetConfig()->MapleExitWindowWebUrl && *Common::GetConfig()->MapleExitWindowWebUrl)
	{
		INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, WinHooks::CreateProcessA_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.OpenMutexA_Logging || Common::GetConfig()->HookToggleInfo.OpenMutexA_Spoof)
	{
		INITWINHOOK("KERNEL32", "OpenMutexA", OpenMutexA_Original, OpenMutexA_t, WinHooks::OpenMutexA_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.NtTerminateProc_Logging)
	{
		INITWINHOOK("NTDLL", "NtTerminateProcess", NtTerminateProcess_Original, NtTerminateProcess_t, WinHooks::NtTerminateProcess_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.RegCreateKeyA_Logging)
	{
		INITWINHOOK("KERNEL32", "RegCreateKeyExA", RegCreateKeyExA_Original, RegCreateKeyExA_t, WinHooks::RegCreateKeyExA_Hook);
	}

	if (Common::GetConfig()->HookToggleInfo.GetProcAddress_Logging)
	{
		INITWINHOOK("KERNEL32", "GetProcAddress", GetProcAddress_Original, GetProcAddress_t, WinHooks::GetProcAddress_Hook);
	}

	if (!bHookWinLibs) return;

	if (!sIP || !sOriginalIP)
	{
#if _DEBUG
		Log("Null IP string passed to Common constructor.");
#endif
		return;
	}

	INITWINHOOK("MSWSOCK", "WSPStartup", WSPStartup_Original, WSPStartup_t, WinHooks::WinSock::WSPStartup_Hook);
}

Common::~Common()
{
#if _DEBUG
	Log("Cleaning up common..");
#endif

	if (this->m_pFakeHsModule)
	{
		// TODO figure out some common library call to put this instead of in dll detach
		// CLogo constructor is pretty good but its not a library call so idk
		this->m_pFakeHsModule->DeleteModule();
	}

	if (this->m_GameSock != INVALID_SOCKET)
	{
#if _DEBUG
		Log("Closing socket..");
#endif

		this->m_ProcTable.lpWSPCloseSocket(this->m_GameSock, nullptr);
		this->m_GameSock = INVALID_SOCKET;
	}
}

void Common::OnThemidaUnpack()
{
	if (Common::GetConfig()->InjectImmediately) return;

	if (this->m_bThemidaUnpacked) return;

	this->m_bThemidaUnpacked = TRUE;

	if (Common::GetConfig()->SleepAfterUnpackDuration)
	{
		Log("Themida unpacked => sleeping for %d milliseconds.", Common::GetConfig()->SleepAfterUnpackDuration);
		Sleep(Common::GetConfig()->SleepAfterUnpackDuration);
	}

#if _DEBUG
	Log("Themida unpacked, editing memory..");
#endif

	this->m_PostMutexFunc();
}
