#pragma once

#include "vec3.h"

class ray {
private:
    point3 origin;
    vec3 direction;

public:
    ray(const point3& o, const vec3& d) {
        origin = o;
        direction = d;
    }

    // getters, read only
    const auto& get_origin() const { return origin; }
    const auto& get_direction() const { return direction; }

    point3 at(float t) const { return origin + t * direction; }
};

