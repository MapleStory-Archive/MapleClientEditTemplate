#include "pch.h"
#include "CppUnitTest.h"
#include "ZArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZArrayUnitTesting)
	{
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
			ZArray<int> arr = ZArray<int>(3);

			for (int i = 0; i < arr.GetCount(); i++)
			{
				arr[i] = 1 + i;
			}

			arr.RemoveAt(1);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(3, arr[1]);
			Assert::AreEqual(2, (int)arr.GetCount());

			ZArray<ZArrayDummyTest> arr2 = ZArray<ZArrayDummyTest>();

			for (int i = 0; i < 5; i++)
			{
				ZArrayDummyTest* pT2 = arr2.InsertBefore(); // keep placing them at the last position

				Assert::AreEqual(42, pT2->test_number); // Test objects construct properly
				Assert::AreEqual(i + 1, (int)arr2.GetCount()); // Test count increases as expected
				Assert::AreEqual(i, (int)arr2.IndexOf(pT2)); // Test index is correct

				pT2[i].test_number = i;
			}

			arr2.RemoveAt(3);

			Assert::AreEqual(4, (int)arr2.GetCount());
			Assert::AreEqual(2, arr2[2].test_number);
			Assert::AreEqual(4, arr2[3].test_number);
		}
	};
}
