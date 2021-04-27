// TemplateTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZXString.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRef.h"

int main()
{
	auto* p10 = new ZRef<long>();
	p10->Alloc();

	auto p20 = ZRef<long>(p10);
	auto p30 = ZRef<long>(p10);
	auto p40 = ZRef<long>(p10);
	auto p50 = ZRef<long>(p10);
	auto p60 = ZRef<long>(p10);

	p10->~ZRef();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
