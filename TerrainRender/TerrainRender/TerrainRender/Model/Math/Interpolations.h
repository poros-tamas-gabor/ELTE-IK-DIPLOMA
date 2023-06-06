#ifndef INTERPOLATIONS_H
#define INTERPOLATIONS_H

///////////////////////////////////////////////////////////////////////////////
// Interpolations.h
// ===============
//
// The AbstractInterpolation abstract class defines the Calculate method that every derived class should implement, 
// as well as the binarySearch_greatestLessOrEqual method to find the greatest element in a vector that is still less than or equal to a given value. 
// The derived class should implement the interpolation process via the Calculate method.
//
// The LinearInterpolation class is a derived class of AbstractInterpolation that implements linear interpolation via the Calculate method.
//
// The CirclularInterpolation class is another derived class of AbstractInterpolation that implements circular interpolation via the Calculate method. 
// It also defines the sgn method to determine the sign of a floating-point number and the angleMod method to calculate the modulus of an angle.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <math.h>
#include "../Persistence/ModelStructs.h"
static const float PI = 2*std::asin(1.0f);

class AbstractInterpolation {
public:
    virtual ~AbstractInterpolation() = default;
    virtual bool Calculate(const std::vector<float>& xValues, const std::vector<Vector3D>& yValues, float x, Vector3D& y, unsigned& index) = 0;
    // find the greatest element in the vector which is still less than or equal to a given value:
    static int binarySearch_greatestLessOrEqual(const std::vector<float>& vec, float value);
};

class LinearInterpolation : public AbstractInterpolation {

public:
    bool Calculate(const std::vector<float>& xValues, const std::vector<Vector3D>& yValues, float x, Vector3D& y, unsigned& index) override;
};

class CirclularInterpolation : public AbstractInterpolation
{
private:
    int sgn(float x);
    float angleMod(float angle);
public:
    bool Calculate(const std::vector<float>& xValues, const std::vector<Vector3D>& yValues, float x, Vector3D& y, unsigned& index) override;
};
#endif