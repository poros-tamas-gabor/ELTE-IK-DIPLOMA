#ifndef SUN_POSITION_H
#define SUN_POSITION_H

#include "Persistence/ModelStructs.h"

class SunPosition
{
private:

	ModelTime		m_currentTime;
	ModelLocation	m_currentLocation;

	double			m_elevation;
	double			m_azimuth;

public:

	SunPosition() = default;

	void	CalculateSunPosition(ModelTime time = { 2023,1,1,12,0,0 }, double lat = 47.497913, double longitude = 19.040236);
	double	GetElevation(void) const;
	double	GetAzimuth(void) const;


private:
	double	CalculateJulianDate(ModelTime time);
	void	CalculateAzimuthAndElevation(double lat, double delta, double lha);
	void	CalculateCelestialCoordinates(double n, double& alpha, double& delta);
	double	CalculateLocalHourAngle(double n, int hour, double alpha, double longitude);

};

#endif