#pragma once
#include "utils.h"

struct Interval {
    // data members
    float min, max;

    // constructors
    Interval() : min(+infinity), max(-infinity) {} // empty: since min > max
    Interval(float minimum, float maximum) : min(minimum), max(maximum) {}

    float interval_size() const { return max - min; }

    bool contains(float x) const { return min <= x && x <= max; }
    bool exclusive_contains(float x) const { return min < x && x < max; }

    //constants
    static const Interval empty;
    static const Interval universe;
};

const Interval Interval::empty = Interval();
const Interval Interval::universe = Interval(-infinity, +infinity);