#include "IInterpolation.h"
#include <cmath>

// find the greatest element in the vector which is still less than or equal to a given value:
int Iitenterpolation::binarySearch_greatestLessOrEqual(const std::vector<float>& vec, float value) {
    int low = 0;
    int high = vec.size() - 1;
    int index = -1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (vec.at(mid) <= value) {
            index = mid;
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return index;
}


bool LinearInterpolation::Calculate(const std::vector<float>& xValues, const std::vector<Vector3D>& yValues, float x, Vector3D& y, unsigned& index)
{
    int i = binarySearch_greatestLessOrEqual(xValues, x);
    if (i == -1)
        return false;
    else if ((i == xValues.size() - 1) && x > xValues.at(i))
        return false;
    else if (i == xValues.size() - 1) 
    {
        index = static_cast<unsigned>(i);
        y = yValues.at(i);
        return true;
    }
    
    index = static_cast<unsigned>(i);
    float x0 = xValues.at(index);
    float x1 = xValues.at(index + 1);
    y = yValues.at(index) * ((x1 - x) / (x1 - x0)) + yValues.at(index + 1) * ((x - x0) / (x1 - x0));
    return true;
}



int CirclularInterpolation::sgn(float x)
{
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}
float CirclularInterpolation::angleMod(float angle)
{
    if (fabs(angle) < PI)
        return angle;
    if (sgn(angle) >= 0)
    {
        return fmod((angle + PI), 2 * PI) - PI;
    }
    else {
        return fmod((angle - PI), 2 * PI) + PI;
    }
}

bool CirclularInterpolation::Calculate(const std::vector<float>& xValues, const std::vector<Vector3D>& yValues,  float x, Vector3D& y, unsigned& index)
{
    //TODO ::
//Compute the difference between the two angles : d = angle2 - angle1
//Adjust the difference to bring it within the range - pi to pi : d = ((d + pi) % (2 * pi)) - pi
//Compute the interpolated angle as : angle = angle1 + t * d
//If angle is outside the range - pi to pi, adjust it by adding or subtracting 2pi : angle = ((angle + pi) % (2pi)) - pi
    int i = binarySearch_greatestLessOrEqual(xValues, x);
    if (i == -1)
        return false;
    else if ((i == xValues.size() - 1) && x > xValues.at(i))
        return false;
    else if (i == xValues.size() - 1)
    {
        index = static_cast<unsigned>(i);
        y = yValues.at(i);
        return true;
    }

    index = static_cast<unsigned>(i);
    float x_i = xValues.at(index);
    float x_ip1 = xValues.at(index + 1);

    Vector3D y_i = yValues.at(index);
    Vector3D y_ip1 = yValues.at(index + 1);


    for (int i = 0; i < 3; i++)
    {
        float diffangle = 0;
        float angle_i = 0;
        switch (i)
        {
        case 0:
            diffangle = y_ip1.x - y_i.x;
            angle_i = y_i.x;
            break;
        case 1:
            diffangle = y_ip1.y - y_i.y;
            angle_i = y_i.y;
            break;
        case 2:
            diffangle = y_ip1.z - y_i.z;
            angle_i = y_i.z;
            break;
        }
        diffangle = angleMod(diffangle);
        float angle = angle_i + diffangle * ((x - x_i) / (x_ip1 - x_i));
        angle = angleMod(angle);
        switch (i)
        {
        case 0:
            y.x = angle;
            break;
        case 1:
            y.y = angle;
            break;
        case 2:
            y.z = angle;
            break;
        }
    }
    return true;
}
