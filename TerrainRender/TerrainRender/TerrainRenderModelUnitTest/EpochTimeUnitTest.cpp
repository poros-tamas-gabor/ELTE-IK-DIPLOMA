#include "pch.h"
#include "CppUnitTest.h"
#include "../TerrainRender/Model/Persistence/EpochTime.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TerrainRenderModelUnitTest
{
	TEST_CLASS(EpochTimeUnitTest)
	{
	public:
		
		TEST_METHOD(EpochTimeConstructor0)
		{
			EpochTime e;
			Assert::AreEqual(e.getNanoseconds(), 0LL);
			Assert::AreEqual(e.getSeconds(), 0LL);

			EpochTime e0 = {100LL,10LL};
			Assert::AreEqual(e0.getSeconds(), 100LL);
			Assert::AreEqual(e0.getNanoseconds(), 10LL);

			EpochTime e2 = { 2000LL,1000000001LL };
			Assert::AreEqual(e2.getNanoseconds(), 1LL);
			Assert::AreEqual(e2.getSeconds(), 2001LL);

			EpochTime e3 = { 2000LL,2000000010L };
			Assert::AreEqual(e3.getNanoseconds(), 10LL);
			Assert::AreEqual(5000000001LL / 1000000000LL, 5LL);
			Assert::AreEqual(e3.getSeconds(), 2002LL);

		}

		TEST_METHOD(EpochTimeOperatorPlus)
		{
			EpochTime e = { 100LL,10LL };
			EpochTime sum = e + EpochTime();
			Assert::AreEqual(sum.getSeconds(), 100LL);
			Assert::AreEqual(sum.getNanoseconds(), 10LL);

			EpochTime e2 = { 2000LL,1000000001LL };

			sum = sum + e2;
			Assert::AreEqual(sum.getSeconds(), 2101LL);
			Assert::AreEqual(sum.getNanoseconds(), 11LL);
		}
		TEST_METHOD(EpochTimeOperatorMinus)
		{
			EpochTime e = { 100LL,10LL };
			EpochTime diff = e - EpochTime();
			Assert::AreEqual(diff.getSeconds(), 100LL);
			Assert::AreEqual(diff.getNanoseconds(), 10LL);

			EpochTime e2 = { 2000LL,1000000001LL };

			diff = e2 - e;
			Assert::AreEqual(diff.getSeconds(), 1900LL);
			Assert::AreEqual(diff.getNanoseconds(), 999999991LL);
		}
		TEST_METHOD(EpochTimeDiff)
		{
			EpochTime e = { 100LL,10LL };
			EpochTime e1 = { 90LL, 50LL };
			double diff = e.diffInMillis(e1);
			Assert::AreEqual(diff, 10 * 1000.0 + 20 / 1000000.0, 0.0001);
		}
		TEST_METHOD(EpochAddMilisec)
		{
			static const long long nsecPerSec = 1000000000L;
			static const long long msecPerSec = 1000L;
			static const long long nsecPermSec = 1000000L;

			EpochTime e = { 100LL,10LL };
			EpochTime sum = e.AddMilliSeconds(2*msecPerSec);
			Assert::AreEqual(sum.getSeconds(), 102LL);
			Assert::AreEqual(sum.getNanoseconds(), 10LL);
			sum = e.AddMilliSeconds(500L);
			Assert::AreEqual(sum.getSeconds(), 100LL);
			Assert::AreEqual(sum.getNanoseconds(), 10LL + 500L * 1000000L);
			
		}
	};
}
