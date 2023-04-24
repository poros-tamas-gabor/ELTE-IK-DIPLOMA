#ifndef I_INTERPOLATION_H
#define I_INTERPOLATION_H
#include <vector>
#include <math.h>
#include "../Persistence/ModelStructs.h"
static const float PI = 2*std::asin(1.0f);

template <typename X>
// find the greatest element in the vector which is still less than the value
static size_t binarySearch(const std::vector<X>& vec, X const& value) {
    size_t low = 0;
    size_t high = vec.size() - 1;
    size_t index = -1;
    while (low <= high) {
        size_t mid = (low + high) / 2;
        if (vec[mid] < value) {
            index = mid;
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return index;
}

template <typename X, typename Y> class Iitenterpolation {
public:
    virtual ~Iitenterpolation() = default;
    virtual bool Calculate(const std::vector<X>& Xs, const std::vector<Y>& containerY, const X& x, Y& y, unsigned& index) = 0;
};

template <typename X, typename Y>
class LinearInterpolation : public Iitenterpolation<X, Y> {

public:
    bool Calculate(const std::vector<X>& Xs, const std::vector<Y>& Ys, const X& x, Y& y, unsigned& index) override {
        index = binarySearch(Xs, x);
        if (index == Xs.size() - 1 || index == -1)
            return false;

        X x0 = Xs.at(index);
        X x1 = Xs.at(index + 1);
        y = Ys.at(index) * ((x1 - x) / (x1 - x0)) + Ys.at(index + 1) * ((x - x0) / (x1 - x0));
        return true;
    }
};

template <typename X>
class CirclularInterpolation : public Iitenterpolation<X, Vector3D>
{
private:
    int sgn(float x)
    {
        return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
    }
    float angleMod(float angle)
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
public:
    bool Calculate(const std::vector<X>& Xs, const std::vector<Vector3D>& Ys, const X& x, Vector3D& y, unsigned& index) override {
        //TODO ::
    //Compute the difference between the two angles : d = angle2 - angle1
    //Adjust the difference to bring it within the range - pi to pi : d = ((d + pi) % (2 * pi)) - pi
    //Compute the interpolated angle as : angle = angle1 + t * d
    //If angle is outside the range - pi to pi, adjust it by adding or subtracting 2pi : angle = ((angle + pi) % (2pi)) - pi
        index = binarySearch(Xs, x);
        if (index == Xs.size() - 1 || index == -1)
            return false;

        X xi = Xs.at(index);
        X xip1 = Xs.at(index + 1);

        Vector3D yi = Ys.at(index);
        Vector3D yip1 = Ys.at(index + 1);


        for (int i = 0; i < 3; i++)
        {
            float diffangle = 0;
            float angle_i = 0;
            switch (i)
            {
            case 0:
                diffangle = yip1.x - yi.x;
                angle_i = yi.x;
                break;
            case 1:
                diffangle = yip1.y - yi.y;
                angle_i = yi.y;
                break;
            case 2:
                diffangle = yip1.z - yi.z;
                angle_i = yi.z;
                break;
            }
            diffangle = angleMod(diffangle);
            float angle = angle_i + diffangle * ((x - xi) / (xip1 - xi));
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
};
#endif