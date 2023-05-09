#include "pch.h"

TEST(BinarySearchTest, Test1) {
    std::vector<float> vec = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float value = 3.5;
    int expected_index = 2;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test2) {

    std::vector<float> vec = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float value = 0.5;
    int expected_index = -1;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test3) {
    std::vector<float> vec = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float value = 5.5;
    int expected_index = 4;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test4) {
    std::vector<float> vec = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float value = 3.0;
    int expected_index = 2;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test5) {
    std::vector<float> vec = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float value = 5.0;
    int expected_index = 4;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test6) {
    std::vector<float> vec = { 1.0, 1.0, 1.0, 1.0, 1.0 };
    float value = 1.0;
    int expected_index = 4;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(BinarySearchTest, Test7) {
    std::vector<float> vec = {  };
    float value = 1.0;
    int expected_index = -1;
    EXPECT_EQ(AbstractInterpolation::binarySearch_greatestLessOrEqual(vec, value), expected_index);
}

TEST(LinearInterpolationTest, Test_Calculate)
{
    LinearInterpolation interpolation;
    Vector3D            y;
    unsigned            index;
    bool                success;

    std::vector<float>     xValues = { 0.0, 1.0, 2.0, 3.0 };
    std::vector<Vector3D>   yValues = { { 1.0, 2.0, 3.0 }, { 2.0, 4.0, 6.0 }, { 3.0, 6.0, 9.0 }, { 4.0, 8.0, 12.0 } };

    success = interpolation.Calculate(xValues, yValues, -1.0, y, index);
    ASSERT_FALSE(success);

    success = interpolation.Calculate(xValues, yValues, 0.0, y, index);
    ASSERT_TRUE(success);
    EXPECT_EQ(index, 0u);
    EXPECT_NEAR(y.x, 1.0, 1e-6);
    EXPECT_NEAR(y.y, 2.0, 1e-6);
    EXPECT_NEAR(y.z, 3.0, 1e-6);

    success = interpolation.Calculate(xValues, yValues, 2.5, y, index);
    ASSERT_TRUE(success);
    EXPECT_EQ(index, 2u);
    EXPECT_NEAR(y.x, 3.5, 1e-6);
    EXPECT_NEAR(y.y, 7.0, 1e-6);
    EXPECT_NEAR(y.z, 10.5, 1e-6);

    success = interpolation.Calculate(xValues, yValues, 3.0, y, index);
    ASSERT_TRUE(success);
    EXPECT_EQ(index, 3u);
    EXPECT_NEAR(y.x, 4.0, 1e-6);
    EXPECT_NEAR(y.y, 8.0, 1e-6);
    EXPECT_NEAR(y.z, 12.0, 1e-6);

    success = interpolation.Calculate(xValues, yValues, 5.0, y, index);
    ASSERT_FALSE(success);
}

TEST(LinearInterpolationTest, Test_CalculateEmpty)
{
    LinearInterpolation interpolation;
    Vector3D            y;
    unsigned            index;
    bool                success;

    std::vector<float>     xValues = {  };
    std::vector<Vector3D>   yValues = {  };

    success = interpolation.Calculate(xValues, yValues, -1.0, y, index);
    ASSERT_FALSE(success);

    success = interpolation.Calculate(xValues, yValues, 5.0, y, index);
    ASSERT_FALSE(success);
}


TEST(CirclularInterpolationTest, Test_Calculate)
{

    CirclularInterpolation  interpolation;
    Vector3D                y;
    float                   x;
    unsigned                index;
    bool                    success;

    // Test case 1
    std::vector<float> xValues{ 0, 1, 2, 3, 4 };
    std::vector<Vector3D> yValues{
        Vector3D(0,         0,          0),
        Vector3D(PI,        PI / 2,     PI / 4),
        Vector3D(-PI,       PI,         0),
        Vector3D(PI / 2,    -PI,        PI / 2),
        Vector3D(0,         - PI / 2,   PI)
    };


    x = -1.0;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_FALSE(success);

    x = 0;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_TRUE(success);
    EXPECT_EQ(index, 0u);
    EXPECT_NEAR(y.x, 0, 1e-6);
    EXPECT_NEAR(y.y, 0, 1e-6);
    EXPECT_NEAR(y.z, 0, 1e-6);

    x = 1.5;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_TRUE(success);
    EXPECT_EQ(index, 1u);
    ASSERT_TRUE((abs(y.x - PI) < 0.001) || (abs(y.x + PI) < 0.001));
    EXPECT_NEAR(y.y, (3 * PI / 4) , 1e-6);
    EXPECT_NEAR(y.z, PI / 8, 1e-6);

    x = 3.5;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_TRUE(success);
    EXPECT_EQ(index, 3u);
    EXPECT_NEAR(y.x, PI / 4, 1e-6);
    EXPECT_NEAR(y.y, -(3 * PI / 4), 1e-6);
    EXPECT_NEAR(y.z,3 * PI / 4, 1e-6);

    x = 4.0;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_TRUE(success);
    EXPECT_EQ(index, 4u);
    EXPECT_NEAR(y.x, 0.0, 1e-6);
    EXPECT_NEAR(y.y, -PI / 2, 1e-6);
    EXPECT_NEAR(y.z, PI, 1e-6);

    x = 5.0;
    success = interpolation.Calculate(xValues, yValues, x, y, index);
    EXPECT_FALSE(success);
}
