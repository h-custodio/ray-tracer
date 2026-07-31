#pragma once
#include "utils.h"

class interval {
private:
    float min, max;

public:
    // constructors
    interval() : min(+infinity), max(-infinity) {} // empty: since min > max
    interval(float minimum, float maximum) : min(minimum), max(maximum) {}

    float get_min() const { return min; }
    float get_max() const { return max; }

    float interval_size() const { return max - min; }

    bool contains(float x) const { return min <= x && x <= max; }
    bool exclusive_contains(float x) const { return min < x && x < max; }

    //constants
    static const interval empty;
    static const interval universe;
};

const interval interval::empty = interval();
const interval interval::universe = interval(-infinity, +infinity);