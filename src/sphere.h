#pragma once

#include "hittable.h"
#include "vec3.h"

class  sphere : public hittable {
private:
    point3 center;
    double radius;
public:
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& record) const override {
        vec3 center_sphere = center - r.get_origin();

        auto a = r.get_direction().magnitude_squared();
        auto b = dot_product(r.get_direction(), center_sphere);
        auto c = center_sphere.magnitude_squared() - radius * radius;

        auto discriminant = b * b - a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrt_discriminant = std::sqrt(discriminant);

        // finds nearest root within acceptable range (tmin < t < tmax)

        // + form of simplified quadratic formula
        auto root = (b - sqrt_discriminant) / a; 
        if (root <= ray_tmin || ray_tmax <= root) {

            // + form of simplified quadratic formula
            root = (b + sqrt_discriminant) / a;

            if (root <= ray_tmin || ray_tmax <= root) {
                return false;
            }
        }

        
        record.t_intersect = root;
        record.point = r.at(record.t_intersect);
        record.normal = (record.point - center) / radius;

        vec3 outward_normal = (record.point - center) / radius;
        record.set_face_normal(r, outward_normal);
    
        return true; 
    }
};


