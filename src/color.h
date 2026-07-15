#pragma once

#include <iostream>

#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // translate the [0,1] component values to the byte range [0,255].
    // turns into hexadecimal form because P6 is being used
    unsigned char red_byte = static_cast<unsigned char>(255.999 * r);
    unsigned char green_byte = static_cast<unsigned char>(255.999 * g);
    unsigned char blue_byte  = static_cast<unsigned char>(255.999 * b);

    // Write out the pixel color components.
    out.put(red_byte);
    out.put(green_byte);
    out.put(blue_byte);
}
