#include "pch.h"
#include "CppUnitTest.h"



TEST(To_FLOAT_WITH_PRECISION_UNITTEST, to_float_with_precision_TestRoundToNearestTenth)
{
	const float value = 3.14159f;
	const float expected = 3.1f;
	const float result = to_float_with_precision(value, 1);
	ASSERT_NEAR(result, expected, 0.0001f);
}

TEST(To_FLOAT_WITH_PRECISION_UNITTEST, to_float_with_precision_TestRoundToNearestHundredth)
{
	const float value = 3.14159f;
	const float expected = 3.14f;
	const float result = to_float_with_precision(value, 2);
	ASSERT_NEAR(result, expected, 0.0001f);
}

TEST(to_float_with_precision, TestRoundToNearestThousandth)
{
	const float value = 3.14159f;
	const float expected = 3.142f;
	const float result = to_float_with_precision(value, 3);
	ASSERT_NEAR(result, expected, 0.0001f);
}

TEST (to_float_with_precision, TestZeroValue)
{
	const float value = 0.0f;
	const float expected = 0.0f;
	const float result = to_float_with_precision(value, 2);
	ASSERT_NEAR(result, expected, 0.0001f);
}

TEST(to_float_with_precision, TestNegativeValue)
{
	const float value = -1.2345f;
	const float expected = -1.2f;
	const float result = to_float_with_precision(value, 1);
	ASSERT_NEAR(result, expected, 0.0001f);
}


TEST(Vector3DUniTest, Constructor)
{
	Vector3D v1;
	ASSERT_FLOAT_EQ(v1.x, 0.0f);
	ASSERT_FLOAT_EQ(v1.y, 0.0f);
	ASSERT_FLOAT_EQ(v1.z, 0.0f);

	Vector3D v2(1, 2, 3);
	ASSERT_FLOAT_EQ(v2.x, 1.0f);
	ASSERT_FLOAT_EQ(v2.y, 2.0f);
	ASSERT_FLOAT_EQ(v2.z, 3.0f);

	std::array<float, 3> arr{ 4, 5, 6 };
	Vector3D v3(arr);
	ASSERT_FLOAT_EQ(v3.x, 4.0f);
	ASSERT_FLOAT_EQ(v3.y, 5.0f);
	ASSERT_FLOAT_EQ(v3.z, 6.0f);
}

TEST(Vector3DUniTest, Operators)
{
	Vector3D v1(1, 2, 3);
	Vector3D v2(2, 3, 4);

	Vector3D result1 = v1 + v2;
	ASSERT_EQ(result1, Vector3D(3, 5, 7));

	Vector3D result2 = v1 - v2;
	ASSERT_EQ(result2, Vector3D(-1, -1, -1));

	Vector3D result3 = v1 * 2;
	ASSERT_EQ(result3, Vector3D(2, 4, 6));

	Vector3D result4 = 2 * v2;
	ASSERT_EQ(result4, Vector3D( 4,6,8 ));

	Vector3D result5 = v1.crossProduct(v2);
	ASSERT_EQ(result5, Vector3D( - 1, 2, -1 ));

	Vector3D result6 = v1.normalize();
	ASSERT_NEAR(result6.x, 0.26726f, 0.0001f);
	ASSERT_NEAR(result6.y, 0.53452f, 0.0001f);
	ASSERT_NEAR(result6.z, 0.80178f, 0.0001f);

	ASSERT_FALSE(v1 == v2);
}

TEST(Vector3DUniTest, SquareLength)
{
	Vector3D v1(1, 2, 3);
	float result1 = v1.squareLength();
	ASSERT_NEAR(result1, 14.0f, 0.0001f);

	Vector3D v2(-1, -2, -3);
	float result2 = v2.squareLength();
	ASSERT_NEAR(result2, 14.0f, 0.0001f);
}

TEST(Vector3DUniTest, ToString)
{
	Vector3D v1(1, 2, 3);
	std::wstring result1 = ToString(v1);
	ASSERT_STREQ(result1.c_str(), L"{ 1, 2, 3 }");

	Vector3D v2(-1, -2, -3);
	std::wstring result2 = ToString(v2);
	ASSERT_STREQ(result2.c_str(), L"{ -1, -2, -3 }");
}
TEST(NormalsInSamePositionsUnitTest, sumNormals_Empty)
{
	NormalsInSamePositions n;
	n.sumNormals();
	const Vector3D expected = Vector3D(0.0f, 0.0f, 0.0f);
	ASSERT_EQ(n.meanNormal, expected);
}
TEST(NormalsInSamePositionsUnitTest, sumNormals)
{
	NormalsInSamePositions n;
	n.normals.push_back(Vector3D(1.0f, 0.0f, 0.0f));
	n.normals.push_back(Vector3D(0.0f, -1.0f, 0.0f));
	n.normals.push_back(Vector3D(0.0f, 0.0f, -1.0f));
	n.sumNormals();

	const Vector3D expected = Vector3D(0.577f, -0.577f, -0.577f);

	ASSERT_NEAR(n.meanNormal.x, expected.x, 0.001f);
	ASSERT_NEAR(n.meanNormal.y, expected.y, 0.001f);
	ASSERT_NEAR(n.meanNormal.z, expected.z, 0.001f);
}




