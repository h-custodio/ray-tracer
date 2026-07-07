#pragma once

#include <cmath>
#include <iostream>
#include <cassert>

// a class to represent the numerical aspects (points and direction) of 3d vectors
class point3 {
    private:
        float vec[3];
    public:
        // default constructor
        point3() {
            vec[0] = 0.0f;
            vec[1] = 0.0f;
            vec[2] = 0.0f;
        }
    
        // constructor
        point3(float x, float y, float z) {
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
        }

        // acccess xyz, read only
        float x() const { return vec[0]; }
        float y() const { return vec[1]; }
        float z() const { return vec[2]; }

        // access xyz, modifiable
        float& x() { return vec[0]; }
        float& y() { return vec[1]; }
        float& z() { return vec[2]; }

        // acccess xyz via array index, read only
        float operator[](int i) const {
            assert(i >= 0 && i < 3 && "i should be in range to access xyz");
            return vec[i];
        }

        // acces xyz via array index, modifiable
        float& operator[](int i) {
            assert(i >= 0 && i < 3 && "i should be in range to access xyz");
            return vec[i];
        }

        // negates the vector and returns a new objecy
        point3 operator-() const {
            return point3(-vec[0], -vec[1], -vec[2]);
        }

        // in-place vector addition    
        point3& operator+=(const point3& other) {
            vec[0] += other.vec[0];
            vec[1] += other.vec[1];
            vec[2] += other.vec[2];
            return *this;
        }

        // in-place vector subtraction    
        point3& operator-=(const point3& other) {
            vec[0] -= other.vec[0];
            vec[1] -= other.vec[1];
            vec[2] -= other.vec[2];
            return *this;
        }

        // in-place scalar multiplication.
        point3& operator*=(float c) {
            vec[0] *= c; 
            vec[1] *= c; 
            vec[2] *= c; 
            return *this;
        }

        // in-place scalar division
        point3& operator/=(float c) {
            assert(c > 0 && "0 division is undefined");
            return *this *= (1.0f / c);
        }

        // returns magnitude(aka length) which is v = sqrt(x^2 + y^2 + z^2)
        float magnitude() const {
            return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        }

        // returns magnitude squared (x^2 + y^2 + z^2), since square root is slower than basic arithmetic
        float magnitude_squared() const {
            return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
        }
};

/*
==========================================
        UTLILITY AND HELPER FUNCTIONS
==========================================
*/

    inline std::ostream& operator<<(std::ostream& out, const point3& a) {
        return out << a.x() << ' ' << a.y() << ' ' << a.z();
    }

    // returns a new object that is composed of the sum of the vector addition
    inline point3 operator+(const point3& a, const point3& b) {
        return point3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z()); 
    }

    // vector x vector
    inline point3 operator*(const point3& a, const point3& b) {
        return point3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z()); 
    }

    
    // vector x vector
    inline point3 operator*(const point3& a, const point3& b) {
        return point3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z()); 
    }
    
    // scalar x vector
    inline point3 operator*(const point3& a, float c) {
        return point3(c * a.x(), c * a.y(), c * a.z()); 
    }

    // vector x scalar
    inline point3 operator*(float c, const point3& a) {
        return point3(c * a); 
    }

    // returns a new vector that is composed of the product of the scalar multiplcation
    inline point3 operator-(const point3& a, const point3& b) {
        return point3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
    }

    inline point3 operator/(const point3& a, float c) {
        return a * (1.0f/c);
    }
 
    // creates a new normalized vector
    point3 normalize(const point3& a) {
        float mag = a.magnitude();
        
        // avoids zero division, xyz are 0
        if (mag == 0) {
            return point3();
        }

        return point3(a.x() / mag, a.y() / mag, a.z() / mag);
    }

    // a . b = ax * bx + ay * by + az * bz 
    inline float dot_product(const point3& a, const point3& b) {
        return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
    }

    // returns new vector 
    // a x b = (ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx)
    inline point3 cross_product(const point3& a, const point3& b) {
        return point3(
            a.y() * b.z() - a.z() * b.y(), 
            a.z() * b.x() - a.x() * b.z(), 
            a.x() * b.y() - a.y() * b.x());
    }