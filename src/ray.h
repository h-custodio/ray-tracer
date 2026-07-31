#pragma once

#include "vec3.h"

class ray {
public:
    // data members
    point3 origin;
    vec3 direction;

    // constructors
    ray() = default;
    ray(const point3& o, const vec3& d) : origin(o), direction(d) {}
    
    point3 at(float t) const { return origin + t * direction; }
};

