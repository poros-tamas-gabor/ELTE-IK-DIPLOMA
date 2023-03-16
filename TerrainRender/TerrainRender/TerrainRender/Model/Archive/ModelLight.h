#ifndef MODEL_LIGHT_H
#define MODEL_LIGHT_H

#include "Persistence/ModelStructs.h"

class ModelLight
{
private:

	ModelTime		_currentTime;
	ModelLocation	_currentLocation;
	
	double			_elevation;
	double			_azimuth;

	//ModelVector4D _ambientColor = { 0.4f, 0.4f, 0.4f, 1.0f };
	//ModelVector4D _diffuseColor = { 0.4f, 0.4f, 0.4f, 1.0f };

public:

	ModelLight() = default;
	

	//void SetAmbientColor(float r, float g, float b, float a);
	//void SetDiffuseColor(float r, float g, float b, float a);

	void CalculateSunPosition(ModelTime time = { 2023,1,1,12,0,0 }, double lat = 47.497913, double longitude = 19.040236);
	
	//ModelVector4D	GetDiffuseColor(void) const;
	//ModelVector4D	GetAmbientColor(void) const;

	double			GetElevation(void) const;
	double			GetAzimuth(void) const;


private:
	double	CalculateJulianDate(ModelTime time);
	void	CalculateAzimuthAndElevation(double lat, double delta, double lha);
	void	CalculateCelestialCoordinates(double n, double& alpha, double& delta);
	double	CalculateLocalHourAngle(double n, int hour, double alpha, double longitude);

};

#endif