#pragma once
#include <Windows.h>
#include <stdio.h>

#define assert_size(x, y) static_assert(x == y, "Static size assert failed.");

//void Log(const char* format, ...);
//void PrintStackTrace(unsigned long dwSize);

/// <summary>
/// Prints to windows debug output.
/// </summary>
static void Log(const char* format, ...)
{
	char buf[1024] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf_s(buf, format, args);

	OutputDebugStringA(buf);

	va_end(args);
}

/// <summary>
/// Print call stack information to debug output
/// </summary>
static void PrintStackTrace(unsigned long dwSize = 10)
{
	// TODO
}