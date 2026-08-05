#pragma once

#include "hittable.h"

class Sphere : public Hittable {
private:
    // data members
    Point3 center;
    float radius;
    std::shared_ptr<Material> mat;
public:
    // constructor
    Sphere(const Point3& center, float radius, std::shared_ptr<Material> mat) 
        : center(center), radius(std::fmax(0,radius)), mat(mat) {}

    // the hit is detetcted by finding the root(s) via simplified quadratic formula
    bool hit(const Ray& r, Interval rp_intersection, HitRecord& record) const override {
        Vec3 center_sphere = center - r.origin;

        auto a = r.direction.magnitude_squared();
        auto b = dot_product(r.direction, center_sphere);
        auto c = center_sphere.magnitude_squared() - radius * radius;

        auto discriminant = b * b - a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrt_discriminant = std::sqrt(discriminant);

        // finds nearest root within acceptable interval
        // - form of simplified quadratic formula
        auto root = (b - sqrt_discriminant) / a; 
        if (!rp_intersection.exclusive_contains(root)) {
            // + form of simplified quadratic formula
            root = (b + sqrt_discriminant) / a;

            if (!rp_intersection.exclusive_contains(root)) {
                return false;
            }
        }

        // updates values of hit_record to hold hit instance 
        record.ray_position = root;
        record.point = r.at(record.ray_position);
        Vec3 outward_normal = (record.point - center) / radius;
        record.set_face_normal(r, outward_normal);
        record.mat = mat;
    
        return true; 
    }
};


