#pragma once
#include <Windows.h>
#include <stdio.h>

#define assert_size(x, y) static_assert(x == y, "Static size assert failed.");

//void Log(const char* format, ...);
//void PrintStackTrace(unsigned long dwSize);

/// <summary>
/// Prints to windows debug output.
/// </summary>
extern void Log(const char* format, ...);