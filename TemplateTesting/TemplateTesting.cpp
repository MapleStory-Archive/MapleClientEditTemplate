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

int main()
{
	auto list = ZList<int>();

	int poo = 40;
	int pee = 30;
	int paa = 20;
	int poo2 = 10;

	list.AddTail(&poo);
	list.AddTail(&pee);
	list.AddTail(&paa);
	list.AddTail(&poo2);

	//list.AddHead(&poo2);

	PINT peepaa = list.GetHeadPosition();
	while (peepaa)
	{
		peepaa = list.GetNext(&peepaa); // TODO fix iteration
	}

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
