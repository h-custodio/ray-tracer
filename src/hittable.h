#pragma once

#include "ray.h"
#include <memory>
#include <vector>


class hit_record {
private:

public:
    point3 point;
    vec3 normal;
    float t_intersect;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        //sets the hit_record normal vector
        //outward_normal is assumed to be normalized when passed

        front_face = dot_product(r.get_direction(), outward_normal) < 0.0f;
        if (front_face) {
            normal = outward_normal;
        } else {
            normal = -outward_normal;
        }
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& record) const = 0;
};

class hittable_list : public hittable {
    std::vector<std::shared_ptr<hittable>> objects;
    
    hittable_list() {};
    hittable_list(std::shared_ptr<hittable> object) { add(object); } 

    void clear() {
        objects.clear();
    }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& record) const override {
        hit_record temp_record;
        bool hit_anything = false;
        auto current_closest = ray_tmax;

        for (const auto& object: objects) {
            if () {
                
            }
        }

        return hit_anything;
    }
};