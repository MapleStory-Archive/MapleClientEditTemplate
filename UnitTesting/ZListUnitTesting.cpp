#include "pch.h"
#include "CppUnitTest.h"
#include "ZList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZListUnitTesting)
	{
	public:
		TEST_METHOD(TraversalTesting)
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

		TEST_METHOD(RemovalTesting)
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

			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			/* Remove all items using tail pointer */
			for (int i = 0; i < 5; i++)
			{
				PINT pTail = list.GetTailPosition();

				/* Verify initial pointer is correct */
				Assert::AreEqual(50 - (10 * i), *pTail);

				list.RemoveAt(pTail);

				/* Verify new head has the correct value */
				if (i == 4)
				{
					/* Last item removed, list is now empty */
					Assert::AreEqual((int*)nullptr, list.GetHeadPosition());
					Assert::AreEqual((int*)nullptr, list.GetTailPosition());
				}
				else
				{
					Assert::AreEqual(40 - (10 * i), *list.GetTailPosition());
				}

				/* Verify remaining size of array is correct */
				Assert::AreEqual((size_t)(4 - i), list.GetCount());
			}

			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			/* Remove item from middle of list */
			PINT pHead = list.GetHeadPosition();
			list.GetNext(&pHead); // scroll forward
			list.GetNext(&pHead); // scroll forward

			list.RemoveAt(pHead); // remove pHead

			Assert::AreEqual((size_t)4, list.GetCount());

			pHead = list.GetHeadPosition();

			/* Verify node with value 30 has been removed */
			Assert::AreEqual(10, *list.GetNext(&pHead));
			Assert::AreEqual(20, *list.GetNext(&pHead));
			Assert::AreEqual(40, *list.GetNext(&pHead));
			Assert::AreEqual(50, *list.GetNext(&pHead));
			Assert::AreEqual((int*)nullptr, list.GetNext(&pHead));

			/* Re-populate list with array item pointers */
			for (int i = 0; i < 5; i++)
			{
				list.AddTail(&a[i]);
			}

			list.RemoveAll(); // TODO determine how to test this
		}

		TEST_METHOD(FindIndexTesting) // TODO finish this
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

			Assert::AreEqual(10, *list.FindIndex(0));
			Assert::AreEqual(30, *list.FindIndex(2));
			Assert::AreEqual(40, *list.FindIndex(3));
			Assert::AreEqual(50, *list.FindIndex(4));
			Assert::AreEqual((int*)nullptr, list.FindIndex(5));
		}

		TEST_METHOD(IndexOfTesting)
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

			/* Verify head and tail indices are correct */
			Assert::AreEqual(0, list.IndexOf(list.GetHeadPosition()));
			Assert::AreEqual(4, list.IndexOf(list.GetTailPosition()));

			PINT pHead = list.GetHeadPosition();

			/* Verify indices are returned correctly */
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(i, list.IndexOf(pHead));
				list.GetNext(&pHead);
			}

			/* Verify passing invalid pointer returns correct error number */
			Assert::AreEqual(ZLIST_INVALID_INDEX, list.IndexOf(pHead));
		}

		// TODO expand this to include finding objects rather than just integer pointers
		TEST_METHOD(FindTesting)
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

			int nNumToFind = 20;
			int nNumToNotFind = 60;

			/* 20 exists in the list, so a pointer to that value should be returned */
			Assert::AreEqual(nNumToFind, *list.Find(&nNumToFind, nullptr));

			/* 60 does not exist in the list, so a nullptr should be returned */
			Assert::AreEqual((int*)nullptr, list.Find(&nNumToNotFind, nullptr));

			nNumToNotFind = 10;

			/* Since the head node is 10 and we are telling it to search after the head node, it should return nullptr */
			Assert::AreEqual((int*)nullptr, list.Find(&nNumToNotFind, list.GetHeadPosition()));

			/* Since 20 is after the head node, that value should be returned */
			Assert::AreEqual(20, *list.Find(&nNumToFind, list.GetHeadPosition()));
		}
	};
}
