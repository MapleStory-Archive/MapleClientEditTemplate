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
	};
}
