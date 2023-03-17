#include "SunPosition.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <time.h>

static const double PI = 3.14159265358979323846;

double SunPosition::GetElevation(void) const { return this->m_elevation; }

double SunPosition::GetAzimuth(void) const { return this->m_azimuth; }

double degToRad(double degree) { return degree * PI / 180.0; }

double radToDeg(double rad) { return rad / PI * 180.0; }

// https://en.wikipedia.org/wiki/Julian_day
// Julian day number calculation
double SunPosition::CalculateJulianDate(ModelTime time) 
{
    // Converting Gregorian calendar date to Julian Day Number
    int jdn = (1461 * (time.year + 4800 + (time.month - 14) / 12)) / 4 +
        (367 * (time.month - 2 - 12 * ((time.month - 14) / 12))) / 12 -
        (3 * ((time.year + 4900 + (time.month - 14) / 12) / 100)) / 4 +
        time.day - 32075;
    // Finding Julian date given Julian day number and time of day
    double jd = jdn + (time.hour - 12.0) / (24.0) + time.min / 1440.0 +
        time.sec / 86400.0;
    return jd;
}

// https://sci-hub.se/https://www.sciencedirect.com/science/article/abs/pii/S0038092X00001560
void SunPosition::CalculateCelestialCoordinates(double n, double& alpha, double& delta) 
{
    double big_omega = 2.1429 * 0.0010394594 * n;

    // The mean longitude of the Sun
    double L = std::fmod(4.895063 + 0.017202791698 * n, 2 * PI);

    // The mean anomaly of the Sun
    double g = std::fmod(6.2400600 + 0.017202791698 * n, 2 * PI);

    // the ecliptic longitude of the Sun is:
    double lambda = L + 0.03341607 * sin(g) + 0.00034894 * sin(2 * g) -
        0.0001134 - 0.0000203 * sin(big_omega);

    // Obliquity of the ecliptic
    double epsilon =
        0.4090928 - degToRad(0.0000004) * n + 0.0000396 * cos(big_omega);

    // Celestial coordinates
    // Right ascension
    alpha = atan2((cos(epsilon) * sin(lambda)), cos(lambda));

    // alpha e [0, 2pi)
    if (alpha < 0.0) {
        alpha += 2 * PI;
    }

    // declination
    delta = asin(sin(epsilon) * sin(lambda));
}

double SunPosition::CalculateLocalHourAngle(double n, int hour, double alpha, double longitude) 
{
    // https://sci-hub.se/10.1016/0038-092x(88)90045-x
    // https://www.astro.umd.edu/~jph/GST_eqn.pdf
    //  Greenwich mean sidereal time
    double gmst = 6.6974243242 + 0.0657098283 * n + hour;
    gmst = std::fmod(gmst, 24.00);

    // Local mean sidereal time
    double lmst = degToRad((15 * gmst + longitude));

    // https://en.wikipedia.org/wiki/Hour_angle
    // local hour angle
    double lha = lmst - alpha;

    if (lha < -PI) {
        lha += 2 * PI;
    }
    else if (lha > PI) {
        lha -= 2 * PI;
    }

    return lha;
}

void SunPosition::CalculateAzimuthAndElevation(double lat, double delta, double lha) 
{
    lat = degToRad(lat);
    // https://en.wikipedia.org/wiki/Solar_zenith_angle
    // https://sci-hub.se/https://www.sciencedirect.com/science/article/abs/pii/S0038092X00001560
    // https://sci-hub.se/10.1016/0038-092x(88)90045-x
    m_elevation =
        asin((sin(lat) * sin(delta)) + (cos(lat) * cos(delta) * cos(lha)));
    m_azimuth = atan2(-sin(lha), tan(delta) * cos(lat) - sin(lat) * cos(lha));
    if (m_azimuth < 0.0) {
        m_azimuth += 2 * PI;
    }
    // https://en.wikipedia.org/wiki/Solar_azimuth_angle
    // http://epa.oszk.hu/03900/03956/00041/pdf/EPA03956_legkor_2015_4_157-161.pdf
}

void SunPosition::SetTimeLocation(std::time_t epochTime, double lat, double longitude) 
{
    this->m_epochTime = epochTime;
    this->m_lat = lat;
    this->m_longitude = longitude;
}

void SunPosition::SetTimeLocationTM(struct tm tm, double lat, double longitude) 
{
    std::time_t utc_time_t = _mkgmtime(&tm);
    this->SetTimeLocation(utc_time_t, lat, longitude);
}

void SunPosition::SetTimeLocation(ModelTime time, double lat, double longitude) 
{
    std::tm utc_time;
    utc_time = { 0 };                        // Set all fields to 0
    utc_time.tm_year = time.year - 1900;     // Year is years since 1900
    utc_time.tm_mon = time.month - 1;        // 0-based indexing
    utc_time.tm_mday = time.day;             // Day of the month
    utc_time.tm_hour = time.hour;            // Day of the month
    utc_time.tm_min = time.min;
    utc_time.tm_sec = time.sec;
    utc_time.tm_isdst = 0;
    _mkgmtime(&utc_time);
    SetTimeLocationTM(utc_time, lat, longitude);
}

void SunPosition::CalculateSunPosition(ModelTime time, double lat, double longitude)
{
    this->m_lat = lat;
    this->m_longitude = longitude;
    double jd;    // JulianDate
    double n;     // the number of days since Greenwich noon
    double alpha; // Right ascension
    double delta; // declination
    double lha;   // local hour angle

    jd = CalculateJulianDate(time);
    // the number of days since Greenwich noon, Terrestrial Time, on 1 January 2000
    n = jd - 2451545.0;
    CalculateCelestialCoordinates(n, alpha, delta);
    lha = CalculateLocalHourAngle(n, time.hour, alpha, longitude);
    CalculateAzimuthAndElevation(lat, delta, lha);
}

void SunPosition::CalculateSunPosition(void) 
{
    double jd;      //julian date
    double n;       // the number of days since Greenwich noon
    double alpha;   // Right ascension
    double delta;   // declination
    double lha;     // local hour angle
    struct tm buf;


    gmtime_s(&buf,&this->m_epochTime);
    ModelTime modelTime = { buf.tm_year + 1900, buf.tm_mon + 1, buf.tm_mday, buf.tm_hour, buf.tm_min };

    jd = CalculateJulianDate(modelTime);
    //  the number of days since Greenwich noon, Terrestrial Time, on 1 January 2000
    n = jd - 2451545.0;
    CalculateCelestialCoordinates(n, alpha, delta);
    lha = CalculateLocalHourAngle(n, modelTime.hour, alpha, this->m_longitude);
    CalculateAzimuthAndElevation(this->m_lat, delta, lha);
}