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

struct SizeTest10
{
public:
	char aPad[10];
};
struct SizeTest20
{
public:
	char aPad[20];
};
struct SizeTest40
{
public:
	char aPad[40];
};
struct SizeTest80
{
public:
	char aPad[80];
};
struct SizeTest160
{
public:
	char aPad[160];
};
struct SizeTest320
{
public:
	char aPad[320];
};

int main()
{
	std::cout << "test begin\r\n";
	ZArray<SizeTest10> arr1 = ZArray<SizeTest10>();
	ZArray<SizeTest20> arr2 = ZArray<SizeTest20>();
	ZArray<SizeTest40> arr3 = ZArray<SizeTest40>();
	ZArray<SizeTest80> arr4 = ZArray<SizeTest80>();
	ZArray<SizeTest160> arr5 = ZArray<SizeTest160>();
	ZArray<SizeTest320> arr6 = ZArray<SizeTest320>();

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 2500; j++)
		{
			switch (rand() % 6)
			{
			case 0:
				arr1.InsertBefore();
				break;
			case 1:
				arr2.InsertBefore();
				break;
			case 2:
				arr3.InsertBefore();
				break;
			case 3:
				arr4.InsertBefore();
				break;
			case 4:
				arr5.InsertBefore();
				break;
			case 5:
				arr6.InsertBefore();
				break;
			}
		}

		std::cout << "arr1 size: " << arr1.GetCount() << "\r\n";

		arr1.RemoveAll();
		arr2.RemoveAll();
		arr3.RemoveAll();
		arr4.RemoveAll();
		arr5.RemoveAll();
		arr6.RemoveAll();
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
