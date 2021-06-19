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
			ZArray<SizeTest10> arr1 = ZArray<SizeTest10>();
			ZArray<SizeTest20> arr2 = ZArray<SizeTest20>();
			ZArray<SizeTest40> arr3 = ZArray<SizeTest40>();
			ZArray<SizeTest80> arr4 = ZArray<SizeTest80>();
			ZArray<SizeTest160> arr5 = ZArray<SizeTest160>();
			ZArray<SizeTest320> arr6 = ZArray<SizeTest320>();

			for (int i = 0; i < 250; i++)
			{
				size_t aSizes[6] = { 0 };

				for (int j = 0; j < 50000; j++)
				{
					int nRand = rand() % 6;
					aSizes[nRand] += 1;
					switch (nRand)
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

				/* Verify array size is correct */
				Assert::AreEqual(aSizes[0], arr1.GetCount());
				Assert::AreEqual(aSizes[1], arr2.GetCount());
				Assert::AreEqual(aSizes[2], arr3.GetCount());
				Assert::AreEqual(aSizes[3], arr4.GetCount());
				Assert::AreEqual(aSizes[4], arr5.GetCount());
				Assert::AreEqual(aSizes[5], arr6.GetCount());

				/* Remove items one at a time */
				while (arr1.GetCount())
				{
					arr1.RemoveAt(arr1.GetCount() - 1);
				}
				while (arr2.GetCount())
				{
					arr2.RemoveAt(arr2.GetCount() - 1);
				}
				while (arr3.GetCount())
				{
					arr3.RemoveAt(arr3.GetCount() - 1);
				}
				while (arr4.GetCount())
				{
					arr4.RemoveAt(arr4.GetCount() - 1);
				}
				while (arr5.GetCount())
				{
					arr5.RemoveAt(arr5.GetCount() - 1);
				}
				while (arr6.GetCount())
				{
					arr6.RemoveAt(arr6.GetCount() - 1);
				}

				/* Verify array size is correct */
				Assert::AreEqual((size_t)0, arr1.GetCount());
				Assert::AreEqual((size_t)0, arr2.GetCount());
				Assert::AreEqual((size_t)0, arr3.GetCount());
				Assert::AreEqual((size_t)0, arr4.GetCount());
				Assert::AreEqual((size_t)0, arr5.GetCount());
				Assert::AreEqual((size_t)0, arr6.GetCount());
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
