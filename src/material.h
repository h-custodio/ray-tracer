#pragma once

#include "hittable.h"
#include "color.h"

class Material {
  public:
    virtual ~Material() = default;

    // r_in: the ray that arrived at the surface.
    // record: details about the collision point
    // attenuation: how much of the ray’s energy/color remains after interacting with the material.
    // scattered: this represents the new ray created after the interaction.
    virtual bool scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) const {
        return false;
    }
};


class Lambertian : public Material {
private:
    Color albedo;
public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        auto scatter_direction = record.normal + random_unit_vector();

        // if the surface normal vector and random unit vector cancel each other,
        // this results zero scatter direction vector, 
        // leading to bad cases like infinities
        // prevented by making scatter direction just the normal
        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }

};