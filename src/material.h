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

class Dielectric : public Material {
private:
    float refraction_index;

    // Schlick's approximation for reflectance
    // simplified formula: R_theta = R0 + (1- R0)(1-cos_theta) ^ 5
    static float reflectance(float one_minus_cos, float r0) {
        auto omc_squared = one_minus_cos * one_minus_cos;
        return r0 + (1.0f - r0) * omc_squared * omc_squared * one_minus_cos;
    }
public:
    Dielectric(float refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0f, 1.0f, 1.0f);

        float ri = record.front_face ? (1.0f / refraction_index) : refraction_index;
        Vec3 unit_direction = unit_vector(r_in.direction);

        auto cos_theta = std::fmin(dot_product(-unit_direction, record.normal), 1.0f);
        auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);

        Vec3 direction;
        bool cannot_refract = ri * sin_theta > 1.0f;

        float r0 = (1.0f - ri) / (1.0f + ri);
        r0 *= r0;

        // if it cannot refract
        if (cannot_refract || reflectance(1.0f - cos_theta, r0) > generate_random(0.0f, 1.0f)) {
            direction = reflect(unit_direction, record.normal);
        } else {
            direction = refract(unit_direction, record.normal, ri, cos_theta);
        }

        scattered = Ray(record.point, direction);
        return true;
    }
};

