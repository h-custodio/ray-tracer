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

float generate_random() {
    // static used to initialized once, reused on later calls
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.5f, 0.5f); 

    // safeguard to prevent getting a result of 0.5f
    float random;
    do {
        // distribution: [-0.5f, 0.5f)
        random = dist(gen); 
    } while (random >= 0.5f);

    return random;
}