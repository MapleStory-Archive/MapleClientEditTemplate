#include "pch.h"
#include "CppUnitTest.h"
#include "ZRef.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZRefUnitTesting)
	{
	public:

		struct ZRefCountedTest : ZRefCounted
		{
			const int test_member_value = 6;
			int test_member;

			ZRefCountedTest()
			{
				test_member = test_member_value;
			}
		};

		struct ZRefUnCountedTest
		{
			const int test_member_value = 6;
			int test_member;

			ZRefUnCountedTest()
			{
				test_member = test_member_value;
			}
		};

		TEST_METHOD(NonCountedReferenceTesting)
		{
			ZRef<ZRefUnCountedTest> pZRefTestBase = ZRef<ZRefUnCountedTest>();

			pZRefTestBase.Alloc();

			ZRefCounted* pBase = reinterpret_cast<ZRefCounted*>(((char*)pZRefTestBase.p) - (sizeof(ZRefCountedDummy<ZRefUnCountedTest>) - sizeof(ZRefUnCountedTest)));

			/* Verify reference count is correct */
			Assert::AreEqual(1, (int)pBase->m_nRef);

			ZRef<ZRefUnCountedTest> pZRefTestInc = ZRef<ZRefUnCountedTest>(&pZRefTestBase);

			/* Verify reference count is correct */
			Assert::AreEqual(2, (int)pBase->m_nRef);

			pZRefTestInc.~ZRef();

			/* Verify reference count is correct */
			Assert::AreEqual(1, (int)pBase->m_nRef);
		}

		TEST_METHOD(CountedReferenceTesting)
		{
			ZRef<ZRefCountedTest> pZRefTestBase = ZRef<ZRefCountedTest>();

			pZRefTestBase.Alloc();

			/* Verify reference count is correct */
			Assert::AreEqual(1, (int)pZRefTestBase->m_nRef);

			ZRef<ZRefCountedTest> pZRefTestInc = ZRef<ZRefCountedTest>(&pZRefTestBase);

			/* Verify reference count is correct */
			Assert::AreEqual(2, (int)pZRefTestInc->m_nRef);
			Assert::AreEqual(2, (int)pZRefTestBase->m_nRef);

			/* Kill the second reference */
			pZRefTestInc.~ZRef();

			/* Verify reference count is correct */
			Assert::AreEqual(1, (int)pZRefTestBase->m_nRef);
		}

		TEST_METHOD(EncapsulatedConstructorTesting)
		{
			ZRef<ZRefCountedTest> pZRefTest = ZRef<ZRefCountedTest>();
			ZRef<ZRefUnCountedTest> pZRefTest2 = ZRef<ZRefUnCountedTest>();

			pZRefTest.Alloc();
			pZRefTest2.Alloc();

			Assert::AreEqual(pZRefTest->test_member_value, pZRefTest->test_member);
			Assert::AreEqual(pZRefTest2->test_member_value, pZRefTest2->test_member);
		}
	};
}
