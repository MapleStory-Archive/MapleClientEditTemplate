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
			ZXString<char> str2 = ZXString<char>(str1);

			Assert::IsTrue(!strcmp(str1, "str1"), TEXT("String assignment constructor failed (char)"));
			Assert::IsTrue(!strcmp(str2, "str1"), TEXT("Copy constructor failed (char)"));

			ZXString<wchar_t> str3 = ZXString<wchar_t>(L"str3");
			ZXString<wchar_t> str4 = ZXString<wchar_t>(str3);

			Assert::IsTrue(!wcscmp(str3, L"str3"), TEXT("String assignment constructor failed (wchar_t)"));
			Assert::IsTrue(!wcscmp(str3, L"str3"), TEXT("Copy constructor failed (wchar_t)"));
		}
	};
}
