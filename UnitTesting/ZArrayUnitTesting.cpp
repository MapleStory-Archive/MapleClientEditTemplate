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
				test_number = 42;
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

		TEST_METHOD(InsertRemoveTesting)
		{
			/* 
				was running into some allocation errors here but they dont seem 
				to be happening 
			*/

			ZArray<SizeTest10> arr1 = ZArray<SizeTest10>();
			ZArray<SizeTest20> arr2 = ZArray<SizeTest20>();
			ZArray<SizeTest40> arr3 = ZArray<SizeTest40>();
			ZArray<SizeTest80> arr4 = ZArray<SizeTest80>();
			ZArray<SizeTest160> arr5 = ZArray<SizeTest160>();
			ZArray<SizeTest320> arr6 = ZArray<SizeTest320>();

			for (int i = 0; i < 250; i++)
			{
				for (int j = 0; j < 50000; j++)
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

				Assert::AreEqual((size_t)0, arr1.GetCount());
				Assert::AreEqual((size_t)0, arr2.GetCount());
				Assert::AreEqual((size_t)0, arr3.GetCount());
				Assert::AreEqual((size_t)0, arr4.GetCount());
				Assert::AreEqual((size_t)0, arr5.GetCount());
				Assert::AreEqual((size_t)0, arr6.GetCount());
			}
		}
	};
}
