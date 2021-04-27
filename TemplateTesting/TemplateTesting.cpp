// TemplateTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZXString.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRef.h"

struct poo : ZRefCounted
{
	int o;
};

int main()
{
	ZRef<ZRefCounted> p1 = ZRef<ZRefCounted>(); // works

	p1.Alloc();

	poo x;
	ZRef<poo> y;

	ZRef<poo> p2 = ZRef<poo>(&x); // works
	ZRef<poo> p3 = ZRef<poo>(&y); // works

	ZRef<ZRefCounted> p4 = ZRef<ZRefCounted>(p1); // this doesnt work????

	/*ZRef<poo> p3 = ZRef<poo>(p1);
	ZRef<poo> p4 = ZRef<poo>(p1);
	ZRef<poo> p5 = ZRef<poo>(p1);
	ZRef<poo> p6 = ZRef<poo>(p1);*/



	auto p10 = new ZRef<long>();
	auto p20 = new ZRef<long>();
	auto p30 = new ZRef<long>();
	auto p40 = new ZRef<long>();
	auto p50 = new ZRef<long>();
	auto p60 = new ZRef<long>();



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
