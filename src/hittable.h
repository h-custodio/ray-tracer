#pragma once

#include <memory>
#include <vector>

#include "ray.h"
#include "interval.h"

// stores information about one hit aka intersection
struct HitRecord {
    Point3 point;   // point of hit/intersaction
    Vec3 normal;    // surface normal 
    float ray_position;  // ray position where the hit occured
    bool front_face;    // is ray from the outside

    // sets the hit_record normal vector
    // outward_normal is assumed to be normalized when passed
    void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot_product(r.direction, outward_normal) < 0.0f;
        if (front_face) {
            normal = outward_normal;    // if ray is from outside of object
        } else {
            normal = -outward_normal;   // if ray is from inside of object
        }
    }
};

// hittable abstract class to use for objects that can interact with the ray
class Hittable {
public:
    //default destructor
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval rp_intersection, HitRecord& record) const = 0;
};

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;
    
    HittableList() {};
    HittableList(std::shared_ptr<Hittable> object) { add(object); } 

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    bool hit(const Ray& r, Interval rp_intersection, HitRecord& record) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        auto current_closest = rp_intersection.max;

        for (const auto& object: objects) {
            // since object is a shared_ptr, and not an object '->' is used
            if (object->hit(r, Interval(rp_intersection.min, current_closest), temp_record)) {
                hit_anything = true;
                current_closest = temp_record.ray_position;
                record = temp_record;
            }
        }

        return hit_anything;
    }
};