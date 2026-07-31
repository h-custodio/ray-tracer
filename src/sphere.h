#pragma once

#include "hittable.h"

class  sphere : public hittable {
private:
    point3 center;
    double radius;
public:
    sphere(const point3& cen, double rad) : center(cen), radius(rad) {}

    bool hit(const ray& r, interval rp_intersection, hit_record& record) const override {
        vec3 center_sphere = center - r.origin;

        auto a = r.direction.magnitude_squared();
        auto b = dot_product(r.direction, center_sphere);
        auto c = center_sphere.magnitude_squared() - radius * radius;

        auto discriminant = b * b - a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrt_discriminant = std::sqrt(discriminant);

        // finds nearest root within acceptable range (tmin < t < tmax)
        // - form of simplified quadratic formula
        auto root = (b - sqrt_discriminant) / a; 
        if (!rp_intersection.exclusive_contains(root)) {
            // + form of simplified quadratic formula
            root = (b + sqrt_discriminant) / a;

            if (!rp_intersection.exclusive_contains(root)) {
                return false;
            }
        }

        record.ray_position = root;
        record.point = r.at(record.ray_position);
        vec3 outward_normal = (record.point - center) / radius;
        record.set_face_normal(r, outward_normal);
    
        return true; 
    }
};


