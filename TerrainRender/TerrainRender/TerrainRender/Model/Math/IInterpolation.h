#ifndef I_INTERPOLATION_H
#define I_INTERPOLATION_H
#include <vector>


template <typename X, typename Y> class Iitenterpolation {
public:
    virtual ~Iitenterpolation() = default;
    virtual bool Calculate(const std::vector<X>& Xs,
        const std::vector<Y>& containerY, const X& x, Y& y) = 0;
};

template <typename X, typename Y>
class LinearInterpolation : public Iitenterpolation<X, Y> {

public:
    bool Calculate(const std::vector<X>& Xs, const std::vector<Y>& Ys,
        const X& x, Y& y) override {
        int index;
        index = binarySearch(Xs, x);
        if (index == Xs.size() - 1 || index == -1)
            return false;

        int x0 = Xs.at(index);
        int x1 = Xs.at(index + 1);
        y = Ys.at(index) * ((x1 - x) / (x1 - x0)) + Ys.at(index + 1) * ((x - x0) / (x1 - x0));
        return true;
    }

private:
    // find the greatest element in the vector which is still less than the value
    int binarySearch(const std::vector<X>& vec, X const& value) {
        int low = 0;
        int high = vec.size() - 1;
        int index = -1;
        while (low <= high) {
            int mid = (low + high) / 2;
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
};
#endif