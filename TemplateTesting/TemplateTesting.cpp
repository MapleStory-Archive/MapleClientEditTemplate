// TemplateTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZXString.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRef.h"
#include "ZtlSecure.h"

int main()
{
	char ac[2];
	unsigned char auc[2];
	
	short as[2];
	unsigned short aus[2];

	int ai[2]; // no need to do long cuz its the same size
	unsigned int aui[2];

	long long all[2];
	unsigned long long aull[2];

	float af[2];
	double ad[2];

	char cr = ZtlSecureFuse<char>(ac, ZtlSecureTear<char>(ac, -45));
	unsigned char ucr = ZtlSecureFuse<unsigned char>(auc, ZtlSecureTear<unsigned char>(auc, -45));

	short sr = ZtlSecureFuse<short>(as, ZtlSecureTear<short>(as, -45));
	unsigned short usr = ZtlSecureFuse<unsigned short>(aus, ZtlSecureTear<unsigned short>(aus, -45));

	int ir = ZtlSecureFuse<int>(ai, ZtlSecureTear<int>(ai, -45));
	unsigned int uir = ZtlSecureFuse<unsigned int>(aui, ZtlSecureTear<unsigned int>(aui, -45));

	long long llr = ZtlSecureFuse<long long>(all, ZtlSecureTear<long long>(all, -45));
	unsigned long long ullr = ZtlSecureFuse<unsigned long long>(aull, ZtlSecureTear<unsigned long long>(aull, -45));

	float fr = ZtlSecureFuse<float>(af, ZtlSecureTear<float>(af, -45));
	double dr = ZtlSecureFuse<double>(ad, ZtlSecureTear<double>(ad, -45));
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
