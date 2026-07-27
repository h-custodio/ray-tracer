#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>

// Common Headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "camera.h"

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = std::numbers::pi_v<float>;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}