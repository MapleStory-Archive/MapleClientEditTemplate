#include <Common.h> // has to be before Windows.h because of linker issues with winsock lib
#include <Windows.h>
#include <iostream>
#include <io.h>

void ErrorBox(const char* format, ...)
{
	char szMessage[1024];

	va_list args;
	va_start(args, format);

	vsnprintf(szMessage, 1023, format, args);
	MessageBoxA(NULL, szMessage, "Launch Error", MB_ICONERROR);

	va_end(args);
}

#define ErrorBoxWithCode(msg) ErrorBox(msg" Error code: %d", GetLastError());

int main()
{
	OutputDebugStringA("Launcher entrypoint.");

	// additional information
	STARTUPINFO siMaple;
	PROCESS_INFORMATION piMaple;

	// Initialize the startup structures
	ZeroMemory(&siMaple, sizeof(siMaple));
	ZeroMemory(&piMaple, sizeof(piMaple));

	siMaple.cb = sizeof(siMaple);

	// Start the child process. 
	BOOL bCreateProc = CreateProcess(
		Common::GetConfig()->MapleExeName,			// name of application
		const_cast<LPSTR>(Common::GetConfig()->MapleStartupArgs), // Command line args
		NULL,               // Process handle not inheritable
		NULL,               // Thread handle not inheritable
		FALSE,              // Set handle inheritance to FALSE
		CREATE_SUSPENDED,   // Creation flags
		NULL,               // Use parent's environment block
		NULL,               // Use parent's starting directory 
		&siMaple,           // Pointer to STARTUPINFO structure
		&piMaple            // Pointer to PROCESS_INFORMATION structure
	);

	if (bCreateProc)
	{
#if !MAPLE_INJECT_USE_IJL
		size_t nLoadDllStrLen;

		if (Common::GetConfig()->DllName)
		{
			nLoadDllStrLen = strlen(Common::GetConfig()->DllName);
		}
		else
		{
			ErrorBoxWithCode("DLL name is null.");
			return FALSE;
		}

		HMODULE hKernel = GetModuleHandleA("Kernel32.dll");

		if (!hKernel)
		{
			ErrorBoxWithCode("Unable to get Kernel32 handle.");
		}

		LPVOID lpLoadLibAddr = (LPVOID)GetProcAddress(hKernel, "LoadLibraryA");

		if (!lpLoadLibAddr)
		{
			ErrorBoxWithCode("Unable to get address for library call.");
		}

		LPVOID lpRemoteStr = VirtualAllocEx(piMaple.hProcess, NULL, nLoadDllStrLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (!lpRemoteStr)
		{
			ErrorBoxWithCode("Unable to allocate memory.");
		}

		BOOL bWriteProc = WriteProcessMemory(piMaple.hProcess, lpRemoteStr, Common::GetConfig()->DllName, nLoadDllStrLen, NULL);

		if (!bWriteProc)
		{
			ErrorBoxWithCode("Unable to attach to MapleStory process.");
		}

		HANDLE hRemThread = CreateRemoteThread(piMaple.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpLoadLibAddr, lpRemoteStr, 0, NULL);

		if (!hRemThread)
		{
			ErrorBoxWithCode("Unable to create remote thread.");
		}
#endif

		DWORD dwRet = ResumeThread(piMaple.hThread);

		if (dwRet == -1)
		{
			ErrorBoxWithCode("Unable to resume thread.");
		}

#if !MAPLE_INJECT_USE_IJL
		// close handle for injected dll
		CloseHandle(hRemThread);
#endif

		// Close process and thread handles. 
		CloseHandle(piMaple.hProcess);
		CloseHandle(piMaple.hThread);

		OutputDebugStringA("End launcher");

		return TRUE;
	}
	else
	{
		ErrorBoxWithCode("Make sure you're running as Administrator and that the launcher is in the same folder as MapleStory.exe.");
		return FALSE;
	}

	return TRUE;
}