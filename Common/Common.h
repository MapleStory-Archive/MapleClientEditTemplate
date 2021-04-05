#pragma once

// Exclude rarely-used stuff from Windows headers
// Important to define this before Windows.h is included in a project because of linker issues with the WinSock2 lib
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <functional>
#include "hooker.h"
#include "logger.h"
#include "winhooks.h"
#include "winhook_types.h"
#include "FakeModule.h"

#define MAPLE_USE_IJL TRUE

/// <summary>
/// 
/// </summary>
class Common
{
private:
	struct Config
	{
	private:
		struct WinHooks
		{
			/* define toggles for logging and other behavior separately */
		public:
			const BOOL OpenMutexA_Spoof = TRUE;

			const BOOL WSPConnect_Logging = TRUE;
			const BOOL NtTerminateProc_Logging = TRUE;
			const BOOL OpenProcess_Logging = FALSE;
			const BOOL CreateProcess_Logging = TRUE;
			const BOOL OpenMutexA_Logging = TRUE;
			const BOOL RegCreateKeyA_Logging = FALSE;
			const BOOL GetProcAddress_Logging = FALSE;

			WinHooks() { }
		};
	public:
		const char* DllName = "LEN.dll";
		const char* MapleExeName = "MapleStory.exe";
		const char* MapleStartupArgs = " GameLaunching 127.0.0.1 8484";

		const char* MapleExitWindowWebUrl = "http";
		const char* MapleWindowClass = "MapleStoryClass";
		const char* MaplePatcherClass = "StartUpDlgClass";
		const char* MapleMutex = "WvsClientMtx";

		const DWORD LocaleSpoofValue = 0;
		const DWORD SleepAfterUnpackDuration = 0;

		const BOOL  ForceWindowedOnStart = TRUE;
		const BOOL  InjectImmediately = FALSE;
		const BOOL  AllowMulticlient = TRUE;

		Common::Config::WinHooks HookToggleInfo = { };

		Config() { }
	};

private:
	static Common* _s_pInstance;
	static Common::Config* _s_pConfig;

public: // public because all the C-style hooks have to access these members
	const char* m_sRedirectIP;
	const char* m_sOriginalIP;

	/* TODO throw all the winsock stuff into its own class */
	SOCKET			m_GameSock;
	WSPPROC_TABLE	m_ProcTable;
	DWORD			m_dwGetProcRetAddr;
	BOOL			m_bThemidaUnpacked;
	FakeModule* m_pFakeHsModule;

	/// <summary>
	/// Gets called when mutex hook is triggered.
	/// </summary>
	std::function<void()> m_PostMutexFunc;

private: // forcing the class to only have one instance, created through CreateInstance
	Common(BOOL bHookWinLibs, std::function<void()> pPostMutexFunc, const char* sIP, const char* sOriginalIP);
	Common() = delete;
	Common(const Common&) = delete;
	Common& operator =(const Common&) = delete;

public:
	~Common();

	/// <summary>
	/// Function called from library hooks.
	/// Most of the time this should be triggered by the Mutex hook, however, in the case that
	/// the Mutex hook does not get triggered then this will be executed by CreateWindowExA
	/// for redundancy. The contents of this function will only be executed once, even if both 
	/// Mutex and CreateWindow hooks are called properly.
	/// </summary>
	void OnThemidaUnpack();

public:
	static void CreateInstance(BOOL bHookWinLibs, std::function<void()> pMutexFunc, const char* sIP, const char* sOriginalIP)
	{
		if (_s_pInstance) return;

		_s_pInstance = new Common(bHookWinLibs, pMutexFunc, sIP, sOriginalIP);
	}

	static Common* GetInstance()
	{
		return _s_pInstance;
	}

	static Config* GetConfig()
	{
		if (!_s_pConfig) _s_pConfig = new Config();

		return _s_pConfig;
	}
};

