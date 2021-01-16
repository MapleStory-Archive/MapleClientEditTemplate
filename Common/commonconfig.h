#pragma once

/*
	global settings config file
	try not to add version-specific settings in here
	use constexpr instead of macros whenever possible
*/

// notes on constexpr vs preprocessor macros
// https://stackoverflow.com/questions/4748083/when-should-you-use-constexpr-capability-in-c11
// https://devblogs.microsoft.com/cppblog/convert-macros-to-constexpr/

constexpr auto MAPLE_INJECT_DLLNAME = "LEN.dll";		// name of dll to inject if above line is commented out
constexpr auto MAPLE_EXE_NAME = "MapleStory.exe";		// name of executable to start
constexpr auto MAPLE_STARTUP_ARGS = " GameLaunching 127.0.0.1 8484";	// maple exe arguments to use on startup (usually needs a space first)
constexpr auto MAPLE_FORCE_WINDOWED = TRUE;				// force windowed login screen
constexpr auto MAPLE_KILL_EXIT_WINDOW = "http";			// string to check against when killing browser startup windows
constexpr auto MAPLE_WINDOW_CLASS = "MapleStoryClass";	// maple window class name -- don't need to change in most cases -- used to force minimized startup window and memory editing redundancy on themida unpacking
constexpr auto MAPLE_PATCHER_CLASS = "StartUpDlgClass";	// name of patcher dialog window to skip -- set to null if none or don't want to skip
constexpr auto MAPLE_MUTEX = "WvsClientMtx";			// name of client mutex -- this is usually WvsClientMtx but might be different in higher versions
constexpr auto MAPLE_LOCALE_SPOOF = 0;					// region/locale ID to spoof -- set to 0 to skip spoofing locale
constexpr auto MAPLE_SLEEP_AFTER_UNPACK = 0;	// number of seconds to sleep when first library call after themida unpacking is triggered (0 for nothing)

#define MAPLE_INJECT_USE_IJL	FALSE	// set this to true to use ijl15 trojan injection method -- you will need to rename your output dll (normally LEN.dll) to ijl15.dll -> this can be done in project properties
#define MAPLE_UNSUSPEND			TRUE	// false to stay suspended after exe is started and dll is injected (normally should be true)
#define MAPLE_INSTAJECT			FALSE	// if true dll will immediately edit memory after injecting (when target exe is unvirtualized)
#define MAPLE_MULTICLIENT		TRUE

// below are used for debug logging
#define MAPLETRACKING_WSPCONN_PRINT		TRUE		// print all wspconnect requests -- useful for finding maple IPs
#define MAPLETRACKING_NT_TERMINATE_PROC TRUE		// hook and log NtTerminateProc
#define MAPLETRACKING_OPEN_PROC			TRUE		// hook and log OpenProcess
#define MAPLETRACKING_CREATE_PROCESS	TRUE		// hook and log CreateProcess(A&W)
#define MAPLETRACKING_OPEN_MUTEXA		TRUE		// hook, log, and spoof OpenMutexA
#define MAPLETRACKING_REGCREATEKEY		TRUE		// hook and log regcreatekey values
#define MAPLETRACKING_GETPROCADDR		FALSE		// hook and log getprocaddr strings. logs all calls from the calling address after mutex is triggered, then it unhooks itself