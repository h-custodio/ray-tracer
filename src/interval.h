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

    float get_min() {
        return min;
    }

    float get_max() {
        return max;
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

    //constants
    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);