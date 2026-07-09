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

        // getters
        const point3& origin() { return origin; }
        const vec3& direction() { return direction; }

        point3 at(float t) { return origin + t * direction; }

};

