#include "pch.h"

static float toRad(float degree)
{
	return degree / 180.0 * PI;
}
TEST(LightTest, Test_UpdateSunPopsition1)
{
	Light light;
	//Azimuth and elevation values are from https://gml.noaa.gov/grad/solcalc/azel.html

	light.UpdateSunPosition(1682670645, 47.497913, 19.040236);

	ASSERT_NEAR(light.GetAzimuth(), toRad(129.87), 0.01);
	ASSERT_NEAR(light.GetElevation(), toRad(47.01), 0.01);
}

TEST(LightTest, Test_UpdateSunPopsition2)
{
	Light light;
	//Azimuth and elevation values are from https://gml.noaa.gov/grad/solcalc/azel.html

	light.UpdateSunPosition(1664534681, 47.497913, 19.040236);

	ASSERT_NEAR(light.GetAzimuth(), toRad(183.5), 0.01);
	ASSERT_NEAR(light.GetElevation(), toRad(39.58), 0.01);
}

TEST(LightTest, Test_UpdateSunPopsition3)
{
	Light light;
	//Azimuth and elevation values are from https://gml.noaa.gov/grad/solcalc/azel.html

	light.UpdateSunPosition(1293202330, 47.497913, 19.040236);

	ASSERT_NEAR(light.GetAzimuth(), toRad(234.25), 0.01);
	ASSERT_NEAR(light.GetElevation(), toRad(0.3), 0.01);
}

TEST(LightTest, Test_UpdateSunPopsition4)
{
	Light light;
	//Azimuth and elevation values are from https://gml.noaa.gov/grad/solcalc/azel.html

	light.UpdateSunPosition(1593594600, 47.497913, 19.040236);

	ASSERT_NEAR(light.GetAzimuth(), toRad(132.82), 0.01);
	ASSERT_NEAR(light.GetElevation(), toRad(58.73), 0.01);
}

TEST(LightTest, Test_UpdateSunPopsition5)
{
	Light light;
	//Azimuth and elevation values are from https://gml.noaa.gov/grad/solcalc/azel.html

	light.UpdateSunPosition(970401600, 47.497913, 19.040236);

	ASSERT_NEAR(light.GetAzimuth(), toRad(206.95), 0.01);
	ASSERT_NEAR(light.GetElevation(), toRad(35.67), 0.01);
}