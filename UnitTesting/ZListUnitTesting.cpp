#include "pch.h"
#include "CppUnitTest.h"
#include "ZList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZListUnitTesting)
	{
	public:
		// TODO maybe split this test up into many smaller tests but im lazy :(

		TEST_METHOD(HeadTailIterationTesting)
		{
			ZList<int> list = ZList<int>();

			int a[5];

			/* Populate array */
			for (int i = 0; i < 5; i++)
			{
				a[i] = 10 + (10 * i);
			}

			/* Populate list and verify tail and head positions, and the list count are updated correctly */
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual((size_t)i, list.GetCount());
				list.AddTail(&a[i]);
				Assert::AreEqual((size_t)i + 1, list.GetCount());

				Assert::AreEqual(10, *list.GetHeadPosition()); // head should never change
				Assert::AreEqual(10 + (i * 10), *list.GetTailPosition()); // tail should change after each insertion
			}

			int iterations = 0;

			/* Verify head->next iteration works properly */
			int* head = list.GetHeadPosition();
			while (head)
			{
				iterations += 1;
				int* value = list.GetNext(&head);

				Assert::AreEqual(iterations * 10, *value);
			}

			/* Verify we looped the correct number of times */
			Assert::AreEqual(5, iterations);

			/* Verify tail->prev iteration works properly */
			iterations = 0;
			int* tail = list.GetTailPosition();
			while (tail)
			{
				iterations += 1;
				int* value = list.GetPrev(&tail);

				Assert::AreEqual(60 - (iterations * 10), *value);
			}

			/* Verify we looped the correct number of times */
			Assert::AreEqual(5, iterations);

			/* Verify no items are linked before head position */
			head = list.GetHeadPosition();
			list.GetPrev(&head);

			Assert::AreEqual((int*)nullptr, head);

			/* Verify no items are linked after tail position */
			tail = list.GetTailPosition();
			list.GetNext(&tail);

			Assert::AreEqual((int*)nullptr, tail);
		}

		TEST_METHOD(ListRemovalTesting)
		{
			ZList<int> list = ZList<int>();

			int a[5];

			/* Populate array */
			for (int i = 0; i < 5; i++)
			{
				a[i] = 10 + (10 * i);
			}

			/* Populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			/* Remove all items using head pointer */
			for (int i = 0; i < 5; i++)
			{
				PINT pHead = list.GetHeadPosition();

				/* Verify initial pointer is correct */
				Assert::AreEqual(10 + (10 * i), *pHead);

				list.RemoveAt(pHead);

				/* Verify new head has the correct value */
				if (i == 4)
				{
					/* Last item removed, list is now empty */
					Assert::AreEqual((int*)nullptr, list.GetHeadPosition());
					Assert::AreEqual((int*)nullptr, list.GetTailPosition());
				}
				else
				{
					Assert::AreEqual(20 + (10 * i), *list.GetHeadPosition());
				}

				/* Verify remaining size of array is correct */
				Assert::AreEqual((size_t)(4 - i), list.GetCount());
			}

			// TODO: Finish the tests below

			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			/* Remove all items using tail pointer */
			for (int i = 0; i < 5; i++)
			{

			}

			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			/* Remove item from middle of list */


			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			list.RemoveAll(); // TODO determine how to test this
		}
	};
}
