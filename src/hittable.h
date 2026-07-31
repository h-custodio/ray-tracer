#pragma once

#include <memory>
#include <vector>

#include "ray.h"
#include "interval.h"

// stores information about one hit aka intersection
class hit_record {
public:
    point3 point;   // point of hit/intersaction
    vec3 normal;    // surface normal 
    float ray_position;  // ray position where the hit occured
    bool front_face;    // is ray from the outside

    // sets the hit_record normal vector
    // outward_normal is assumed to be normalized when passed
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot_product(r.direction, outward_normal) < 0.0f;
        if (front_face) {
            normal = outward_normal;    // if ray is from outside of object
        } else {
            normal = -outward_normal;   // if ray is from inside of object
        }
    }
};

// hittable abstract class to use for objects that can interact with the ray
class hittable {
public:
    //default destructor
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval rp_intersection, hit_record& record) const = 0;
};

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;
    
    hittable_list() {};
    hittable_list(std::shared_ptr<hittable> object) { add(object); } 

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    bool hit(const ray& r, interval rp_intersection, hit_record& record) const override {
        hit_record temp_record;
        bool hit_anything = false;
        auto current_closest = rp_intersection.get_max();

        for (const auto& object: objects) {
            // since object is a shared_ptr, and not an object '->' is used
            if (object->hit(r, interval(rp_intersection.get_min(), current_closest), temp_record)) {
                hit_anything = true;
                current_closest = temp_record.ray_position;
                record = temp_record;
            }
        }

        return hit_anything;
    }
};