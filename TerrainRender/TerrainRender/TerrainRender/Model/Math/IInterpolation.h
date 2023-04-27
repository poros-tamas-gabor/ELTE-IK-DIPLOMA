#ifndef I_INTERPOLATION_H
#define I_INTERPOLATION_H
#include <vector>
#include <math.h>
#include "../Persistence/ModelStructs.h"
static const float PI = 2*std::asin(1.0f);




class Iitenterpolation {
public:
    virtual ~Iitenterpolation() = default;
    virtual bool Calculate(const std::vector<double>& xValues, const std::vector<Vector3D>& yValues, double x, Vector3D& y, unsigned& index) = 0;
    // find the greatest element in the vector which is still less than or equal to a given value:
    static int binarySearch_greatestLessOrEqual(const std::vector<double>& vec, double value);
};

class LinearInterpolation : public Iitenterpolation {

public:
    bool Calculate(const std::vector<double>& xValues, const std::vector<Vector3D>& yValues, double x, Vector3D& y, unsigned& index) override;
};

class CirclularInterpolation : public Iitenterpolation
{
private:
    int sgn(float x);
    float angleMod(float angle);
public:
    bool Calculate(const std::vector<double>& xValues, const std::vector<Vector3D>& yValues, double x, Vector3D& y, unsigned& index) override;
};
#endif