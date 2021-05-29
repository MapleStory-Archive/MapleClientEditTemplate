// TemplateTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZXString.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRef.h"
#include "ZtlSecure.h"
#include <TSecType.h>
#include "ZArray.h"
#include "ZList.h"

struct test
{
	int val;
	~test()
	{
		std::cout << "destroying";
	}
};

int main()
{
	auto list = ZList<test>();

	for (int i = 0; i < 4; i++)
	{
		auto n = new test();
		n->val = i;
		list.AddTail(n);
	}

	int x = 1;
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
