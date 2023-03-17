#ifndef SUN_POSITION_H
#define SUN_POSITION_H

#include "Persistence/ModelStructs.h"
#include <ctime>
class SunPosition {
private:
    std::time_t m_epochTime   = 1664534690;
    double m_lat             = 47.497913;
    double m_longitude       = 19.040236;

    double m_elevation;
    double m_azimuth;

public:
    SunPosition() = default;

    void SetTimeLocation(ModelTime time,        double lat = 47.497913, double longitude = 19.040236);
    void SetTimeLocation(std::time_t epochTime, double lat = 47.497913, double longitude = 19.040236);
    void SetTimeLocationTM(struct tm tm,          double lat = 47.497913, double longitude = 19.040236);
    void CalculateSunPosition(  ModelTime time, double lat = 47.497913,   double longitude = 19.040236);
    void CalculateSunPosition(void);
    double GetElevation(void) const;
    double GetAzimuth(void) const;

private:
    double CalculateJulianDate(ModelTime time);
    void CalculateAzimuthAndElevation(double lat, double delta, double lha);
    void CalculateCelestialCoordinates(double n, double& alpha, double& delta);
    double CalculateLocalHourAngle(double n, int hour, double alpha,
        double longitude);
};
#endif