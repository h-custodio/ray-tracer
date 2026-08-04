#pragma once

#include <iostream>

#include "vec3.h"
#include "interval.h"

using Color = Vec3;

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    static const Interval intensity(0.000f, 0.999f);

    // translate the [0,1] component values to the byte range [0,255].
    // turns into hexadecimal form because PPM P6 is being used
    unsigned char red_byte   = static_cast<unsigned char>(255.999f * intensity.clamp(r));
    unsigned char green_byte = static_cast<unsigned char>(255.999f * intensity.clamp(g));
    unsigned char blue_byte  = static_cast<unsigned char>(255.999f * intensity.clamp(b));

    // write out the pixel color components.
    out.put(red_byte);
    out.put(green_byte);
    out.put(blue_byte);
}
