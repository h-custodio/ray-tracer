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
        // prevented by making scatter direction just the normal if they cancel out
        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }

};

class Metal : public Material {
private:
    Color albedo;
    float fuzz;
public:
    Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        auto reflection_direction = reflect(r_in.direction, record.normal);
        // offset reflection by random scaled to fuzz
        reflection_direction = unit_vector(reflection_direction) + (fuzz * random_unit_vector()); 

        scattered = Ray(record.point, reflection_direction);
        attenuation = albedo;

        // only return true if the scatter is above surface
        return (dot_product(scattered.direction, record.normal) > 0);
    }

};