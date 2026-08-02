#pragma once

#include "vec3.h"

struct Ray {
    // data members
    Point3 origin;
    Vec3 direction;

    // constructors
    Ray() = default;
    Ray(const Point3& o, const Vec3& d) : origin(o), direction(d) {}
    
    Point3 at(float t) const { return origin + t * direction; }
};

