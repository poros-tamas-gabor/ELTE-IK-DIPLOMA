#include "pch.h"
TEST(EpochTimeUnitTest, Constructor)
{
	EpochTime e;
	EXPECT_EQ(e.getNanoseconds(), 0LL);
	EXPECT_EQ(e.getSeconds(), 0LL);

	EpochTime e0 = { 100LL,10LL };
	EXPECT_EQ(e0.getSeconds(), 100LL);
	EXPECT_EQ(e0.getNanoseconds(), 10LL);

	EpochTime e2 = { 2000LL,1000000001LL };
	EXPECT_EQ(e2.getNanoseconds(), 1LL);
	EXPECT_EQ(e2.getSeconds(), 2001LL);

	EpochTime e3 = { 2000LL,2000000010L };
	EXPECT_EQ(e3.getNanoseconds(), 10LL);
	EXPECT_EQ(5000000001LL / 1000000000LL, 5LL);
	EXPECT_EQ(e3.getSeconds(), 2002LL);
}

TEST(EpochTimeUnitTest, OperatorPlus)
{
	EpochTime e = { 100LL,10LL };
	EpochTime sum = e + EpochTime();

	EXPECT_EQ(sum.getSeconds(), 100LL);
	EXPECT_EQ(sum.getNanoseconds(), 10LL);
	EpochTime e2 = { 2000LL,1000000001LL };

	sum = sum + e2;
	EXPECT_EQ(sum.getSeconds(), 2101LL);
	EXPECT_EQ(sum.getNanoseconds(), 11LL);
}

TEST(EpochTimeUnitTest, OperatorMinus)
{
	EpochTime e = { 100LL,10LL };
	EpochTime diff = e - EpochTime();
	EXPECT_EQ(diff.getSeconds(), 100LL);
	EXPECT_EQ(diff.getNanoseconds(), 10LL);

	EpochTime e2 = { 2000LL,1000000001LL };
	diff = e2 - e;
	EXPECT_EQ(diff.getSeconds(), 1900LL);
	EXPECT_EQ(diff.getNanoseconds(), 999999991LL);
}

TEST(EpochTimeUnitTest, diffInMillis)
{
	EpochTime e = { 100LL,10LL };
	EpochTime e1 = { 90LL, 50LL };
	double diff = e.diffInMillis(e1);
	EXPECT_NEAR(diff, 10 * 1000.0 + 20 / 1000000.0, 0.0001);
}

TEST(EpochTimeUnitTest, AddMilisec)
{
	static const long long nsecPerSec = 1000000000L;
	static const long long msecPerSec = 1000L;
	static const long long nsecPermSec = 1000000L;
	EpochTime e = { 100LL,10LL };
	EpochTime sum = e.AddMilliSeconds(2 * msecPerSec);
	EXPECT_EQ(sum.getSeconds(), 102LL);
	EXPECT_EQ(sum.getNanoseconds(), 10LL);
	sum = e.AddMilliSeconds(500L);
	EXPECT_EQ(sum.getSeconds(), 100LL);
	EXPECT_EQ(sum.getNanoseconds(), 10LL + 500L * 1000000L);
}
