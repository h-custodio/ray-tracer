#pragma once

#include <iostream>
#include <cassert>

class vec3 {
public:
    // data member
    float vec[3];

    // default constructor, xyz are 0
    vec3() : vec {} {}

    // constructor
    vec3(float x, float y, float z) : vec(x, y, z) {}

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
    inline vec3 operator-() const {
        return vec3(-vec[0], -vec[1], -vec[2]);
    }

    // in-place vector addition    
    inline vec3& operator+=(const vec3& other) {
        vec[0] += other[0];
        vec[1] += other[1];
        vec[2] += other[2];
        return *this;
    }

    // in-place vector subtraction    
    inline vec3& operator-=(const vec3& other) {
        vec[0] -= other[0];
        vec[1] -= other[1];
        vec[2] -= other[2];
        return *this;
    }

    // in-place scalar multiplication
    inline vec3& operator*=(float scalar) {
        vec[0] *= scalar; 
        vec[1] *= scalar; 
        vec[2] *= scalar; 
        return *this;
    }

    // in-place scalar division
    inline vec3& operator/=(float scalar) {
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
};

// ========== Operators ========== //

inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z()); 
}

// vector x vector
inline vec3 operator*(const vec3& a, const vec3& b) {
    return vec3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z()); 
}

// vector x scalar
inline vec3 operator*(const vec3& a, float scalar) {
    return vec3(scalar * a.x(), scalar * a.y(), scalar * a.z()); 
}

// scalar x vector
inline vec3 operator*(float scalar, const vec3& a) {
    return a * scalar;
}

inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
}

inline vec3 operator/(const vec3& a, float scalar) {
    assert(scalar != 0.0f);
    return a * (1.0f / scalar);
}

// prints out xyz of a vector
inline std::ostream& operator<<(std::ostream& out, const vec3& a) {
    return out << a.x() << ' ' << a.y() << ' ' << a.z();
}

// ========== Vector Math ========== //

// a . b = ax * bx + ay * by + az * bz 
inline float dot_product(const vec3& a, const vec3& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

// a x b = (ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx)
inline vec3 cross_product(const vec3& a, const vec3& b) {
    return vec3(
        a.y() * b.z() - a.z() * b.y(), 
        a.z() * b.x() - a.x() * b.z(), 
        a.x() * b.y() - a.y() * b.x());
}

// creates a normalized vector aka unit vector
// xyz values range from -1 to 1
inline vec3 normalize(const vec3& v) {
    float mag = v.magnitude();
    
    // avoids zero division, xyz are 0
    if (mag == 0.0f) {
        return vec3();
    }

    return v / mag;
}

// ========== Alias ========== //

// point3 is an alias of vec3 for geometric clarity.
using point3 = vec3;