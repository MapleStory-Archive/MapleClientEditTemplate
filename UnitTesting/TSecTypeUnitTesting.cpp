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
			TSecType<char> ts_c = TSecType<char>();
			TSecType<short> ts_s = TSecType<short>();
			TSecType<long> ts_l = TSecType<long>();
			TSecType<double> ts_d = TSecType<double>();

			Assert::IsTrue(ts_c.GetData() == 0, TEXT("Char constructor failed to initialize data to zero."));
			Assert::IsTrue(ts_s.GetData() == 0, TEXT("Short constructor failed to initialize data to zero."));
			Assert::IsTrue(ts_l.GetData() == 0, TEXT("Long constructor failed to initialize data to zero."));
			Assert::IsTrue(ts_d.GetData() == 0.0, TEXT("Double constructor failed to initialize data to zero."));

			ts_c = 1;
			ts_s = 1;
			ts_l = 1;
			ts_d = 1.0;

			Assert::IsTrue(ts_c.GetData() == 1, TEXT("Char SetData() failed to set data to 1."));
			Assert::IsTrue(ts_s.GetData() == 1, TEXT("Short SetData() failed to set data to 1."));
			Assert::IsTrue(ts_l.GetData() == 1, TEXT("Long SetData() failed to set data to 1."));
			Assert::IsTrue(ts_d.GetData() == 1.0, TEXT("Double SetData() failed to set data to 1."));
		}

		TEST_METHOD(OperatorOverloadTesting)
		{
			TSecType<long> ts_l = TSecType<long>();
			ts_l = 1;

			TSecType<long> ts_l1 = ts_l;

			Assert::IsTrue(ts_l1.GetData() == ts_l.GetData(), TEXT("Operator = overload failed."));

			Assert::IsTrue(ts_l == ts_l1, TEXT("Operator == overload failed."));
			Assert::IsFalse(ts_l != ts_l1, TEXT("Operator != overload failed."));

			ts_l *= 2;

			Assert::IsTrue(ts_l == 2, TEXT("Operator *= overload failed."));

			ts_l += ts_l;

			Assert::IsTrue(ts_l == 4, TEXT("Operator += overload failed."));

			ts_l += 4;

			Assert::IsTrue(ts_l == 8, TEXT("Operator += overload failed."));

			int x = ts_l + 4 + ts_l;

			Assert::IsTrue(x == 20, TEXT("Operator + overload failed."));

			int y = ts_l - 4 - ts_l;

			Assert::IsTrue(y == -4, TEXT("Operator - overload failed."));

			ts_l -= 4;

			Assert::IsTrue(ts_l == 4, TEXT("Operator -= overload failed."));
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

			Assert::IsTrue(sp5 == &sp4, TEXT("Operator == overload failed."));
			Assert::IsFalse(sp5 != &sp4, TEXT("Operator != overload failed."));
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

			SECPOINT sp1 = SECPOINT(15, 20);
			tagPOINT tp1 = tagPOINT(sp);

			Assert::IsTrue(sp == &tp, TEXT("SECPOINT does not equal tagPOINT"));
			Assert::IsFalse(sp != &tp, TEXT("SECPOINT equals tagPOINT"));

			tagPOINT tp2 = sp;

			Assert::IsTrue(sp == &tp2, TEXT("tagPOINT operator overload failed."));
		}
	};
}
