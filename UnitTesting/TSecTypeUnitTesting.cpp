#include "pch.h"
#include "CppUnitTest.h"
#include "TSecType.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonUnitTesting
{
	TEST_CLASS(TSecTypeUnitTesting)
	{
	public:

		TEST_METHOD(ConstructorTesting)
		{

		}
	};

	TEST_CLASS(SECPOINTUnitTesting)
	{
	public:

		TEST_METHOD(ConstructorTesting)
		{
			SECPOINT sp = SECPOINT();

			Assert::IsTrue(sp.x.GetData() == 0, TEXT("X does not equal 0"));
			Assert::IsTrue(sp.y.GetData() == 0, TEXT("X does not equal 0"));

			SECPOINT sp2 = SECPOINT(15, 20);

			Assert::IsTrue(sp2.x.GetData() == 15, TEXT("X does not equal 15"));
			Assert::IsTrue(sp2.y.GetData() == 20, TEXT("Y does not equal 20"));

			SECPOINT sp3 = &sp2;

			Assert::IsTrue(sp2.x.GetData() == sp3.x.GetData(), TEXT("SECPOINT2.X does not equal SECPOINT3.X"));
			Assert::IsTrue(sp2.y.GetData() == sp3.y.GetData(), TEXT("SECPOINT2.Y does not equal SECPOINT3.Y"));

			tagPOINT tp = tagPOINT();
			tp.x = 4;
			tp.y = 3;

			SECPOINT sp4 = &tp;

			Assert::IsTrue(tp.x == sp4.x.GetData(), TEXT("tagPOINT->SECPOINT constructor failed (X value incorrect)"));
			Assert::IsTrue(tp.y == sp4.y.GetData(), TEXT("tagPOINT->SECPOINT constructor failed (Y value incorrect)"));

			SECPOINT sp5;

			sp5 = &tp;

			Assert::IsTrue(tp.x == sp5.x.GetData(), TEXT("SECPOINT = tagPOINT operator overload failed (X value incorrect)"));
			Assert::IsTrue(tp.y == sp5.y.GetData(), TEXT("SECPOINT = tagPOINT operator overload failed (Y value incorrect)"));
		}

		TEST_METHOD(EqualsOperatorTesting)
		{
			SECPOINT sp = SECPOINT(15, 20);

			Assert::IsTrue(sp.x.GetData() == 15, TEXT("X does not equal 15"));
			Assert::IsTrue(sp.y.GetData() == 20, TEXT("Y does not equal 20"));
		}

		TEST_METHOD(TagPointTesting)
		{
			SECPOINT sp;
			tagPOINT tp = tagPOINT();

			tp.x = 1;
			tp.y = 2;

			Assert::IsTrue(tp.x == 1 && tp.y == 2);

			sp = &tp;
			Assert::IsTrue(sp.x == tp.x, TEXT("TagPoint and SecPoint X-Value missmatch"));
			Assert::IsTrue(sp.y == tp.y, TEXT("TagPoint and SecPoint Y-Value missmatch"));


		}
	};
}
