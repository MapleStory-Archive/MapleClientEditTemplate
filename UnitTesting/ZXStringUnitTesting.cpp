#include "pch.h"
#include "CppUnitTest.h"
#include <ZXString.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(ZXStringUnitTesting)
	{
	public:
		TEST_METHOD(ConstructorTesting)
		{
			ZXString<char> str1 = ZXString<char>("str1");
			ZXString<char> str2 = ZXString<char>(str2);

			Assert::IsTrue(strstr(str1, "str1"), TEXT("String assignment constructor failed"));
			Assert::IsTrue(strstr(str2, "str1"), TEXT("Copy constructor failed"));
		}
	};
}
