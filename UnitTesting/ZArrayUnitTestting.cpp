#include "pch.h"
#include "CppUnitTest.h"
#include "ZArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZArrayUnitTesting)
	{
	public:
		TEST_METHOD(ConstructorTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			Assert::IsTrue(arr.GetCount() == 10);
		}

		TEST_METHOD(SubscriptOperatorTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			for (int i = 0; i < 10; i++)
			{
				arr[i] = i * 5;
			}

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(i * 5, arr[i]);
			}
		}

		TEST_METHOD(NextPrevTesting)
		{
			ZArray<int> arr = ZArray<int>(10);

			for (int i = 0; i < 10; i++)
			{
				arr[i] = i * 10;
			}

			PINT pHead = arr.GetHeadPosition();

			Assert::AreEqual(90, *pHead);

			for (int i = 9; i > 0; i--)
			{
				Assert::AreEqual(i * 10, *pHead);
				Assert::AreEqual(i, (int)arr.IndexOf(pHead));

				arr.GetNext(&pHead);
			}

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(i * 10, *pHead);
				arr.GetPrev(&pHead);
			}
		}
	};
}
