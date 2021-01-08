#pragma once

/*
	global settings config file
	try not to add version-specific settings in here
*/

#define MAPLE_INJECT_USE_IJL						// comment this line out to use other injection methods (not ijl15 trojan)
#define MAPLE_INJECT_DLLNAME	"LEN.dll"			// name of dll to inject if above line is commented out
#define MAPLE_EXE_NAME			"MapleStory.exe"	// name of executable to start
#define MAPLE_STARTUP_ARGS		" GameLaunching 127.0.0.1 8484"	// maple exe arguments to use on startup (usually needs a space first)
#define MAPLE_UNSUSPEND			TRUE				// false to stay suspended after exe is started and dll is injected (normally should be true)

#define MAPLE_INSTAJECT			FALSE				// if true dll will immediately edit memory after injecting (when target exe is unvirtualized)
#define MAPLE_FORCE_WINDOWED	TRUE				// force windowed login screen
#define MAPLE_KILL_EXIT_WINDOW	"http"				// string to check against when killing browser startup windows
#define MAPLE_WINDOW_CLASS		"MapleStoryClass"	// maple window class name -- don't need to change in most cases -- used to force minimized startup window and memory editing redundancy on themida unpacking
#define MAPLE_PATCHER_CLASS		"StartUpDlgClass"	// name of patcher dialog window to skip -- set to null if none or don't want to skip
#define MAPLE_MUTEX				"WvsClientMtx"		// name of client mutex -- this is usually WvsClientMtx but might be different in higher versions
#define MAPLE_LOCALE_SPOOF		874					// region/locale ID to spoof -- set to 0 to skip spoofing locale

// below are used for debug logging
#define MAPLETRACKING_WSPCONN_PRINT		FALSE	// print all wspconnect requests -- useful for finding maple IPs
#define MAPLETRACKING_NT_TERMINATE_PROC TRUE	// hook and log NtTerminateProc
#define MAPLETRACKING_OPEN_PROC			TRUE	// hook and log OpenProcess
#define MAPLETRACKING_CREATE_PROCESS	TRUE	// hook and log CreateProcess(A&W)
#define MAPLETRACKING_OPEN_MUTEXA		TRUE	// hook, log, and spoof OpenMutexA
#define MAPLETRACKING_REGCREATEKEY		TRUE	// hook and log regcreatekey values
#define MAPLETRACKING_SLEEP_ON_UNPACK	0		// number of seconds to sleep when first library call after themida unpacking is triggered (0 for nothing) 