#include "pch.h"
#include "CppUnitTest.h"
#include "ZtlSecure.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZtlSecureUnitTesting)
	{
		template<typename T>
		void ZtlSecureTest(T expected_val)
		{
			T arr[2];

			T result = ZtlSecureFuse<T>(arr, ZtlSecureTear<T>(arr, expected_val));

			Assert::AreEqual(expected_val, result);
		}
	public:
		TEST_METHOD(FuseTearChar)
		{
			ZtlSecureTest<char>(-45);
		}
		TEST_METHOD(FuseTearShort)
		{
			ZtlSecureTest<short>(-45);
		}
		TEST_METHOD(FuseTearInt)
		{
			ZtlSecureTest<int>(-45);
		}
		TEST_METHOD(FuseTearLongLong)
		{
			ZtlSecureTest<long long>(-45);
		}
		TEST_METHOD(FuseTearFloat)
		{
			ZtlSecureTest<float>(-45);
		}
		TEST_METHOD(FuseTearDouble)
		{
			ZtlSecureTest<double>(-45);
		}
		TEST_METHOD(MacroValidation)
		{
			Assert::AreEqual((unsigned int)0xBAADF00D, ZTLSECURE_CHECKSUM);
			Assert::AreEqual(5, ZTLSECURE_ROTATION);
		}

		// =============== SECRECT TESTING
	};

	TEST_CLASS(SECRECTUnitTesting)
	{
		TEST_METHOD(SetRectTest)
		{
			SECRECT s = SECRECT(10, 15, 20, 30);

			Assert::AreEqual(s.GetLeft(), 10);
			Assert::AreEqual(s.GetTop(), 15);
			Assert::AreEqual(s.GetRight(), 20);
			Assert::AreEqual(s.GetBottom(), 30);
		}
		TEST_METHOD(SetRectEmptyTest)
		{
			SECRECT s = SECRECT(10, 15, 20, 30);

			Assert::AreEqual(s.GetLeft(), 10);
			Assert::AreEqual(s.GetTop(), 15);
			Assert::AreEqual(s.GetRight(), 20);
			Assert::AreEqual(s.GetBottom(), 30);

			s.SetRectEmpty();

			Assert::AreEqual(s.GetLeft(), 0);
			Assert::AreEqual(s.GetTop(), 0);
			Assert::AreEqual(s.GetRight(), 0);
			Assert::AreEqual(s.GetBottom(), 0);
		}
		TEST_METHOD(PutGetRectTest)
		{
			SECRECT s = SECRECT();

			s.PutRight(10);
			s.PutLeft(15);
			s.PutTop(20);
			s.PutBottom(25);

			Assert::AreEqual(s.GetRight(), 10);
			Assert::AreEqual(s.GetLeft(), 15);
			Assert::AreEqual(s.GetTop(), 20);
			Assert::AreEqual(s.GetBottom(), 25);
		}
		TEST_METHOD(IsRectEmptyTest)
		{
			SECRECT s = SECRECT(10, 15, 7, 4);

			Assert::AreEqual(TRUE, s.IsRectEmpty());

			s.SetRect(7, 4, 10, 15);

			Assert::AreEqual(FALSE, s.IsRectEmpty());
		}
	};
}
