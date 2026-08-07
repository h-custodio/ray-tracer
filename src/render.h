#pragma once

#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "camera.h"
#include "material.h"

// decleration
inline std::ofstream setup_ppm6(const std::string& file_name, const Camera& cam);

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

    std::vector<Color> render(const Camera& cam, const HittableList& world) const {        
        std::vector <Color> framebuffer(cam.get_image_width() * cam.get_image_height());
        
        // // file setup
        // std::string file_name = "display.ppm";
        // auto output_file = setup_ppm6(file_name, cam);

        // render pixel grid
        for (int row = 0; row < cam.get_image_height(); row++) {
            std::clog << "\rScanlines remaining: " << (cam.get_image_height() - row) << ' ' << std::flush;

            for (int col = 0; col < cam.get_image_width(); col++) {

                framebuffer[row * cam.get_image_width() + col] = random_sampling(row, col, cam, world) / samples_per_pixel; 
                //Color color_averaged = random_sampling(row, col, cam, world) / samples_per_pixel; 
               
                // write_color(output_file, color_averaged);
            }
        }

        std::clog << "\rDone!                 \n";

        // // Close the PPM file
        // output_file.close();
        return framebuffer;
    }
};

inline std::ofstream setup_ppm6(const std::string& file_name, const Camera& cam) {
    // // check if the file already existss
    // if (std::filesystem::exists(file_name)) {
    //     std::cerr << "Error: " << file_name << " already exists\n";
    //     return 1;
    // }

    // open file
    std::ofstream output_file(file_name, std::ios::binary);
    if (!output_file.is_open()) {
        throw std::runtime_error("Error opening the file");
    }

    // PPM header setup

    // Prints the P6 header to signify ppm format 
    output_file << "P6\n"; 
    output_file << cam.get_image_width() << ' ' << cam.get_image_height() << "\n";

    // The highest value a color channel can have. 256 possible intensities (0 to 255) for each color.
    output_file << 255 << "\n";

    std::cout << "Setup complete\n"; 
    return output_file;
}

inline void write_file(std::vector<Color>& framebuffer, const std::string& file_name, const Camera& cam) {
    auto output_file = setup_ppm6(file_name, cam);
    
    for (const Color& pixel : framebuffer) {
        write_color(output_file, pixel);
    }
}