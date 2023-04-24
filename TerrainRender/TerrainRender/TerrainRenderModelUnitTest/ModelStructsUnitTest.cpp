#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TerrainRenderModelUnitTest
{

	TEST_CLASS(To_FLOAT_WITH_PRECISION_UNITTEST)
	{
		TEST_METHOD(to_float_with_precision_TestRoundToNearestTenth)
		{
			const float value = 3.14159f;
			const float expected = 3.1f;
			const float result = to_float_with_precision(value, 1);
			Assert::AreEqual(result, expected, 0.0001f);
		}

		TEST_METHOD(to_float_with_precision_TestRoundToNearestHundredth)
		{
			const float value = 3.14159f;
			const float expected = 3.14f;
			const float result = to_float_with_precision(value, 2);
			Assert::AreEqual(result, expected, 0.0001f);
		}

		TEST_METHOD(to_float_with_precision_TestRoundToNearestThousandth)
		{
			const float value = 3.14159f;
			const float expected = 3.142f;
			const float result = to_float_with_precision(value, 3);
			Assert::AreEqual(result, expected, 0.0001f);
		}

		TEST_METHOD(to_float_with_precision_TestZeroValue)
		{
			const float value = 0.0f;
			const float expected = 0.0f;
			const float result = to_float_with_precision(value, 2);
			Assert::AreEqual(result, expected, 0.0001f);
		}

		TEST_METHOD(to_float_with_precision_TestNegativeValue)
		{
			const float value = -1.2345f;
			const float expected = -1.2f;
			const float result = to_float_with_precision(value, 1);
			Assert::AreEqual(result, expected, 0.0001f);
		}
	};
	TEST_CLASS(Vector3DUniTest)
	{
	public:
		TEST_METHOD(Vector3D_constructor)
		{
			Vector3D v1;
			Assert::AreEqual(v1.x, 0.0f);
			Assert::AreEqual(v1.y, 0.0f);
			Assert::AreEqual(v1.z, 0.0f);

			Vector3D v2(1, 2, 3);
			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 2.0f);
			Assert::AreEqual(v2.z, 3.0f);

			std::array<float, 3> arr{ 4, 5, 6 };
			Vector3D v3(arr);
			Assert::AreEqual(v3.x, 4.0f);
			Assert::AreEqual(v3.y, 5.0f);
			Assert::AreEqual(v3.z, 6.0f);
		}

		TEST_METHOD(Vector3D_operators)
		{
			Vector3D v1(1, 2, 3);
			Vector3D v2(2, 3, 4);

			Vector3D result1 = v1 + v2;
			Assert::AreEqual(result1, { 3,5,7 });

			Vector3D result2 = v1 - v2;
			Assert::AreEqual(result2, { -1,-1,-1 });

			Vector3D result3 = v1 * 2;
			Assert::AreEqual(result3, { 2,4,6 });

			Vector3D result4 = 2 * v2;
			Assert::AreEqual(result4, { 4,6,8 });

			Vector3D result5 = v1.crossProduct(v2);
			Assert::AreEqual(result5, { -1,2,-1 });

			Vector3D result6 = v1.normalize();
			Assert::AreEqual(result6.x, 0.26726f, 0.0001f);
			Assert::AreEqual(result6.y, 0.53452f, 0.0001f);
			Assert::AreEqual(result6.z, 0.80178f, 0.0001f);

			Assert::IsFalse(v1 == v2);
		}

		TEST_METHOD(Vector3D_SquareLength)
		{
			Vector3D v1(1, 2, 3);
			float result1 = v1.squareLength();
			Assert::AreEqual(result1, 14.0f, 0.0001f);

			Vector3D v2(-1, -2, -3);
			float result2 = v2.squareLength();
			Assert::AreEqual(result2, 14.0f, 0.0001f);
		}

		TEST_METHOD(Vector3D_ToString)
		{
			Vector3D v1(1, 2, 3);
			std::wstring result1 = ToString(v1);
			Assert::AreEqual(result1.c_str(), L"{ 1, 2, 3 }");

			Vector3D v2(-1, -2, -3);
			std::wstring result2 = ToString(v2);
			Assert::AreEqual(result2.c_str(), L"{ -1, -2, -3 }");
		}
	};

	TEST_CLASS(NormalsInSamePositionsUnitTest)
	{
	public:
		TEST_METHOD(NormalsInSamePositions_sumNormals_Empty)
		{
			NormalsInSamePositions n;
			n.sumNormals();
			const Vector3D expected = Vector3D(0.0f, 0.0f, 0.0f);
			Assert::AreEqual(n.meanNormal, expected);
		}
		TEST_METHOD(NormalsInSamePositions_sumNormals)
		{
			NormalsInSamePositions n;
			n.normals.push_back(Vector3D(1.0f, 0.0f, 0.0f));
			n.normals.push_back(Vector3D(0.0f, -1.0f, 0.0f));
			n.normals.push_back(Vector3D(0.0f, 0.0f, -1.0f));
			n.sumNormals();

			const Vector3D expected = Vector3D(0.577f, -0.577f, -0.577f);

			Assert::AreEqual(n.meanNormal.x, expected.x, 0.001f);
			Assert::AreEqual(n.meanNormal.y, expected.y, 0.001f);
			Assert::AreEqual(n.meanNormal.z, expected.z, 0.001f);
		}

	};

}

