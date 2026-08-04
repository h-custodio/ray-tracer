#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>
#include <random>

// ========== Constants ========== //

const float infinity = std::numeric_limits<float>::infinity();
const float pi = std::numbers::pi_v<float>;

// ========== Utility Functions ========== //

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}

// generate random float from [min, max)
float generate_random(float min, float max) {
    // static used to initialized once, reused on later calls
    static std::mt19937 gen;

    std::uniform_real_distribution<float> dist(min, max); 

    return dist(gen); 
}