#include "pch.h"
#include "CppUnitTest.h"
#include "ZArray.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZArrayUnitTesting)
	{
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

		struct ZArrayDummyTest
		{
		public:
			int test_number;

			ZArrayDummyTest()
			{
				this->test_number = 42;
			}

			ZArrayDummyTest(ZArrayDummyTest* pToCopy)
			{
				this->test_number = pToCopy->test_number;
			}
		};

	public:
		TEST_METHOD(ConstructorTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			Assert::IsTrue(arr.GetCount() == 10);
		}

		TEST_METHOD(GetHeadTesting)
		{
			ZArray<int> arr = ZArray<int>(2);

			arr[0] = 10;
			arr[1] = 20;

			auto pHead = arr.GetHeadPosition();
			Assert::AreEqual(20, *pHead);

			arr.RemoveAt(pHead);

			pHead = arr.GetHeadPosition();
			Assert::AreEqual(10, *pHead);
		}

		TEST_METHOD(SubscriptOperatorTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			for (int i = 0; i < arr.GetCount(); i++)
			{
				arr[i] = i * 5;
			}

			for (int i = 0; i < arr.GetCount(); i++)
			{
				Assert::AreEqual(i * 5, arr[i]);
			}
		}

		TEST_METHOD(NextPrevTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			for (int i = 0; i < arr.GetCount(); i++)
			{
				arr[i] = i * 10;
				Assert::AreEqual(i * 10, arr[i]);
			}

			PINT pHead = arr.GetHeadPosition();

			Assert::AreEqual(90, *pHead);

			for (int i = arr.GetCount() - 1; i > 0; i--)
			{
				Assert::AreEqual(i * 10, *pHead);
				Assert::AreEqual(i, (int)arr.IndexOf(pHead));

				arr.GetNext(&pHead);
			}

			for (int i = 0; i < arr.GetCount(); i++)
			{
				Assert::AreEqual(i * 10, *pHead);
				arr.GetPrev(&pHead);
			}
		}

		// observing the duration of this function vs the one below will showcase just how much
		// faster it is to use maples block/pool allocation method instead of malloc/free
		/*TEST_METHOD(MassMallocTesting)
		{
			for (int i = 0; i < 250; i++)
			{
				for (int j = 0; j < 50000; j++)
				{
					PVOID pAlloc = malloc(100);
					free(pAlloc);
				}
			}
		}*/

		TEST_METHOD(InsertRemoveTesting)
		{
			ZArray<ZArrayDummyTest> arr = ZArray<ZArrayDummyTest>();

			for (int i = 0; i < 10; i++)
			{
				auto item = arr.InsertBefore();
				Assert::AreEqual((size_t)i + 1, arr.GetCount());
				Assert::AreEqual(42, item->test_number);
			}
		}

		TEST_METHOD(EqualsOperatorTesting)
		{
			ZArray<ZArrayDummyTest> arr1 = ZArray<ZArrayDummyTest>();
			ZArray<ZArrayDummyTest> arr2 = ZArray<ZArrayDummyTest>();

			for (int i = 0; i < 100; i++)
			{
				auto x = arr1.InsertBefore();
				x->test_number += i;
			}

			arr2 = arr1;

			for (int i = 0; i < 100; i++)
			{
				Assert::AreEqual(42 + i, arr1[i].test_number);
				Assert::AreEqual(arr1[i].test_number, arr2[i].test_number);
			}
		}

		TEST_METHOD(ReallocTesting)
		{
			size_t initial_size = sizeof(ZArrayDummyTest) * 10;
			ZArray<ZArrayDummyTest> arr = ZArray<ZArrayDummyTest>(initial_size);

			size_t uCount = arr.GetCount();

			arr.MakeSpace(initial_size);
			Assert::AreEqual(uCount, arr.GetCount());
			arr.MakeSpace(initial_size * 2);
			Assert::AreEqual(uCount * 2, arr.GetCount());

			arr.MakeSpace(initial_size * 3);
			Assert::AreEqual(uCount * 4, arr.GetCount()); // *4 cuz size is always doubled
		}
	};
}
