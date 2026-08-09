#pragma once

#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "camera.h"
#include "material.h"

class Renderer {
private:
    // data member
    int samples_per_pixel;  // amount of rays sampled per pixel, defaulted at 200
    int max_ray_depth;      // maximum amount of times the ray bounces into scene, defaulted at 50

    // ========== Functions ========== //
    
    Color ray_color(const Ray& r, const Hittable& world, int depth) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0) {
            return Color(0,0,0);
        }

        HitRecord record;

        // if ray hits something in front of camera
        if (world.hit(r, Interval(0.001f, infinity), record)) {
            Ray scattered;
            Color attenuation;
            
            // if false then the object absorbs the ray, stopping further bounces
            // The camera learns nothing beyond the point of absorption
            if (record.mat->scatter(r, record, attenuation, scattered)) {
                // ray recursively bounces until ray misses an object
                return attenuation * ray_color(scattered, world, depth - 1);
            }

            return Color(0.0f, 0.0f, 0.0f);
        } 

        // sky gradient
        auto direction_unit_vector = unit_vector(r.direction);
        auto a = 0.5f * (direction_unit_vector.y() + 1.0f);
        //                  startValue                    endValue
        return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
    }

    Point3 defocus_disk_sample(const Camera& cam) const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return cam.get_camera_center() + (p[0] * cam.get_defocus_disk_horizontal()) + (p[1] * cam.get_defocus_disk_vertical());
    }

    Color random_sampling(int row, int col, const Camera& cam, const HittableList& world) const {
        Color color_accumulator;
        for (int i = 0; i < samples_per_pixel; i++) {
            // initialize pixel center first then,
            // randomly offset the xy to get random point within the pixel
            auto pixel_position = cam.get_first_pixel_location() 
                + (col * cam.get_horizontal_pixel_delta()) 
                + (row * cam.get_vertical_pixel_delta())
                + (generate_random(-0.5f, 0.5f) * cam.get_horizontal_pixel_delta()) 
                + (generate_random(-0.5f, 0.5f) * cam.get_vertical_pixel_delta());
        
            // initialize ray
            Point3 ray_origin;
            if (cam.get_defocus_angle() <= 0.0f) { 
                ray_origin = cam.get_camera_center();
            }
            else {
                ray_origin = defocus_disk_sample(cam);
            }

            auto ray_direction = pixel_position - ray_origin;
            Ray r(ray_origin, ray_direction);

            color_accumulator += ray_color(r, world, max_ray_depth);
        }

        return color_accumulator;
    }
public:
    // default constructor
    Renderer() : Renderer(200, 50) {}

    Renderer(int samples_per_pixel, int max_ray_depth) : samples_per_pixel(samples_per_pixel), max_ray_depth(max_ray_depth) {
        if (samples_per_pixel <= 0 || max_ray_depth <= 0) {
            throw std::invalid_argument("Invalid renderer settings");
        }
    }

    std::vector<Color> render(const Camera& cam, const HittableList& world, int begin, int end) const {        
        std::vector<Color> framebuffer(cam.get_image_width() * (end - begin));
        
        // render pixel grid
        for (int row = begin; row < end; row++) {
            // std::clog << "\rScanlines remaining: " << (end - row) << ' ' << std::flush;

            for (int col = 0; col < cam.get_image_width(); col++) {

                framebuffer[(row-begin) * cam.get_image_width() + col] = random_sampling(row, col, cam, world) / samples_per_pixel; 
            }
        }

        // std::clog << "\rDone!                 \n";

        return framebuffer;
    }
};

