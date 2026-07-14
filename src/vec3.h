#pragma once

#include <cmath>
#include <iostream>
#include <cassert>
#include <stdexcept>

class vec3 {
private:
    float vec[3];
public:
    // default constructor
    vec3() {
        vec[0] = 0.0f;
        vec[1] = 0.0f;
        vec[2] = 0.0f;
    }

    // constructor
    vec3(float x, float y, float z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }

    // acccess xyz, read only (easier readability)
    float x() const { return vec[0]; }
    float y() const { return vec[1]; }
    float z() const { return vec[2]; }

    // access xyz, modifiable
    float& x() { return vec[0]; }
    float& y() { return vec[1]; }
    float& z() { return vec[2]; }

    // acccess xyz via array index, read only
    float operator[](int i) const {
        assert(i >= 0 && i < 3 && "i should be 0, 1 or 2");
        return vec[i];
    }

    // acces xyz via array index, modifiable
    float& operator[](int i) {
        assert(i >= 0 && i < 3 && "i should be 0, 1 or 2");
        return vec[i];
    }

    // negates the vector and returns a new object
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
        if (scalar == 0.0f) {
            throw std::invalid_argument("division by zero");
        }

        return *this *= (1.0f / scalar);
    }

    // returns magnitude(aka length) which is v = sqrt(x^2 + y^2 + z^2)
    inline float magnitude() const {
        return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    }

    // returns magnitude squared (x^2 + y^2 + z^2), since square root is slower than basic arithmetic
    inline float magnitude_squared() const {
        return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    }
};


// point3 is an alias of vec3 for geometric clarity.
using point3 = vec3;


/*
==================
    Operators
==================
*/

// returns a new object that is composed of the sum of the vector addition
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

// returns a new vector that is composed of the product of the scalar multiplcation
inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
}

inline vec3 operator/(const vec3& a, float scalar) {
    if (std::fabs(scalar) < 1e-8f) {
        throw std::invalid_argument("division by zero");
    }

    return a * (1.0f / scalar);
}

inline bool operator==(const vec3& a, const vec3& b) {
    return a.x() == b.x() &&
           a.y() == b.y() &&
           a.z() == b.z();
}

// for float approximation
inline bool near_equal(const vec3& a, const vec3& b, float epsilon = 1e-6f) {
    return std::fabs(a.x() - b.x()) < epsilon &&
           std::fabs(a.y() - b.y()) < epsilon &&
           std::fabs(a.z() - b.z()) < epsilon;
}

// prints out xyz of a vector
inline std::ostream& operator<<(std::ostream& out, const vec3& a) {
    return out << a.x() << ' ' << a.y() << ' ' << a.z();
}

/*
==================
    Vector Math
==================
*/

// creates a new normalized vector
inline vec3 normalize(const vec3& a) {
    float mag = a.magnitude();
    
    // avoids zero division, xyz are 0
    if (mag == 0) {
        return vec3();
    }

    return vec3(a.x() / mag, a.y() / mag, a.z() / mag);
}

// a . b = ax * bx + ay * by + az * bz 
inline float dot_product(const vec3& a, const vec3& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

// returns new vector 
// a x b = (ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx)
inline vec3 cross_product(const vec3& a, const vec3& b) {
    return vec3(
        a.y() * b.z() - a.z() * b.y(), 
        a.z() * b.x() - a.x() * b.z(), 
        a.x() * b.y() - a.y() * b.x());
}