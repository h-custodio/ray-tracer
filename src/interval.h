#pragma once
#include "utils.h"

class interval {
private:
    float min, max;

public:
    interval() {
        //remember that y is inversed
        min = +infinity;
        max = -infinity;
    }

    interval(float minimum, float maximum) {
        min = minimum;
        max = maximum;
    }

    float interval_size() const {
        return max - min;
    }

    bool contains(float x) {
        return min <= x && x <= max;
    }

    bool exclusive_contains(float x) {
        return min < x && x < max;
    }
};