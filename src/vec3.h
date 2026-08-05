#pragma once

#include <iostream>
#include <cassert>
#include <cmath>

#include "utils.h"

struct Vec3 {
    // data member
    float vec[3];

    // default constructor, xyz are 0
    Vec3() : vec {} {}

    // constructor
    Vec3(float x, float y, float z) : vec(x, y, z) {}

    // access xyz, read only (easier readability)
    float x() const { return vec[0]; }
    float y() const { return vec[1]; }
    float z() const { return vec[2]; }

    // acccess xyz via array index, read only
    float operator[](int i) const {
        assert(i >= 0 && i < 3 && "i should be 0, 1 or 2");
        return vec[i];
    }

    // negates the vector and returns a object
    inline Vec3 operator-() const {
        return Vec3(-vec[0], -vec[1], -vec[2]);
    }

    // in-place vector addition    
    inline Vec3& operator+=(const Vec3& other) {
        vec[0] += other[0];
        vec[1] += other[1];
        vec[2] += other[2];
        return *this;
    }

    // in-place vector subtraction    
    inline Vec3& operator-=(const Vec3& other) {
        vec[0] -= other[0];
        vec[1] -= other[1];
        vec[2] -= other[2];
        return *this;
    }

    // in-place scalar multiplication
    inline Vec3& operator*=(float scalar) {
        vec[0] *= scalar; 
        vec[1] *= scalar; 
        vec[2] *= scalar; 
        return *this;
    }

    // in-place scalar division
    inline Vec3& operator/=(float scalar) {
        assert(scalar != 0.0f);
        return *this *= (1.0f / scalar);
    }

    // returns magnitude squared
    inline float magnitude_squared() const {
        return (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    }

    // returns magnitude(aka length) which is v = sqrt(x^2 + y^2 + z^2)
    inline float magnitude() const {
        return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    }

    // return true if the vector is close to zero in all dimensions
    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(vec[0]) < s) && (std::fabs(vec[1]) < s) && (std::fabs(vec[2]) < s);
    }
};

// ========== Operators ========== //

inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z()); 
}

// vector x vector
inline Vec3 operator*(const Vec3& a, const Vec3& b) {
    return Vec3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z()); 
}

// vector x scalar
inline Vec3 operator*(const Vec3& a, float scalar) {
    return Vec3(scalar * a.x(), scalar * a.y(), scalar * a.z()); 
}

// scalar x vector
inline Vec3 operator*(float scalar, const Vec3& a) {
    return a * scalar;
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
}

inline Vec3 operator/(const Vec3& a, float scalar) {
    assert(scalar != 0.0f);
    return a * (1.0f / scalar);
}

// prints out xyz of a vector
inline std::ostream& operator<<(std::ostream& out, const Vec3& a) {
    return out << a.x() << ' ' << a.y() << ' ' << a.z();
}

// ========== Vector Math ========== //

// a . b = ax * bx + ay * by + az * bz 
inline float dot_product(const Vec3& a, const Vec3& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

// a x b = (ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx)
inline Vec3 cross_product(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y() * b.z() - a.z() * b.y(), 
        a.z() * b.x() - a.x() * b.z(), 
        a.x() * b.y() - a.y() * b.x());
}

// creates a normalized vector aka unit vector (magnitude of 1)
// xyz values range [-1, 1]
inline Vec3 unit_vector(const Vec3& v) {
    float mag = v.magnitude();
    
    assert(mag >= 1e-8f && "avoid close to zero division");

    return v / mag;
}

static Vec3 random_vector(float min, float max) {
    return Vec3(generate_random(min, max), generate_random(min, max), generate_random(min, max));
}

inline Vec3 random_unit_vector() {
    // keep generating random vectors until criteria satisfied
    while (true) {
        auto rv = random_vector(-1.0f, 1.0f);
        auto rv_mag_squared = rv.magnitude_squared();

        // checks if magnitude squared is effectively zero
        // and within unit sphere
        if (1e-38f < rv_mag_squared && rv_mag_squared <= 1) {
            return rv / std::sqrt(rv_mag_squared); // normalize the random vector
        }
    }
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = random_unit_vector();

    // valid if in the same hemisphere as normal
    if (dot_product(normal, on_unit_sphere) > 0.0f) { 
        return on_unit_sphere;
    }

    // invert if inside object
    return -on_unit_sphere;
}

// returns vector that represents the direction of ray reflected on metal
// n is a normal, which is a unit vector
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    // this is the same as v + 2*b
    // where b = -dot_product(v, n) * n; 
    return v - 2 * dot_product(v, n) * n;
}

inline Vec3 refract(const Vec3& unit_v, const Vec3& n, float eta_over_etap) {
    auto cos_theta = std::fmin(dot_product(-unit_v, n), 1.0f);

    // refracted ray, R_prime_perpendicular = eta / eta_prime * (R + (-R dot n) * n)
    // where base R represents ray inound, unit_v
    Vec3 perpendicular = eta_over_etap * (unit_v + cos_theta * n); 

    // refracted ray, R_prime_parallel = -sqrt(1 - | R_prime_perpendicular | ^ 2 * n)
    Vec3 parallel = -std::sqrt(std::fabs(1.0f - perpendicular.magnitude_squared())) * n;

    return perpendicular + parallel;
}

// ========== Alias ========== //

// point3 is an alias of Vec3 for geometric clarity.
using Point3 = Vec3;