#pragma once

#include "camera.h"

class Renderer {
private:
    // data member
    int sample_amount = 200; // amount of rays sampled per pixel, defaulted at 200
    int max_depth = 50; // maximum amount of times the ray bounces into scene

    Color normal_to_color(const Vec3& unit_vector) {
        return Color((unit_vector.x() + 1.0f) / 2.0f,
            (unit_vector.y() + 1.0f) / 2.0f, 
            (unit_vector.z() + 1.0f) / 2.0f);
    }

    Color ray_color(const Ray& r, const Hittable& world, int depth) {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0) {
            return Color(0,0,0);
        }

        HitRecord record;

        // if ray hits something in front of camera
        if (world.hit(r, Interval(0.001f, infinity), record)) {
            Vec3 direction = random_on_hemisphere(record.normal);

            // ray recursively bounces until ray misses an object
            return 0.5f * ray_color(Ray(record.point, direction), world, depth - 1);
        } 

        // sky gradient
        auto direction_unit_vector = normalize(r.direction);
        auto a = 0.5f * (direction_unit_vector.y() + 1.0f);

        //                  startValue                    endValue
        return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
    }

    Color random_sampling(int row, int col, int sample_amount, Camera cam, const HittableList& world) {
        Color color_accumulator;
        for (int i = 0; i < sample_amount; i++) {
            // initialize pixel center first then,
            // randomly offset the xy to get random point within the pixel
            auto pixel_position = cam.get_first_pixel_location() 
                + (col * cam.get_horizontal_pixel_delta()) 
                + (row * cam.get_vertical_pixel_delta())
                + (generate_random(-0.5f, 0.5f) * cam.get_horizontal_pixel_delta()) 
                + (generate_random(-0.5f, 0.5f) * cam.get_vertical_pixel_delta());
        
            // initialize ray
            auto ray_direction = pixel_position - cam.get_camera_position();
            Ray r(cam.get_camera_position(), ray_direction);

            color_accumulator += ray_color(r, world, max_depth);
        }

        return color_accumulator;
    }
public:
    int render(Camera cam, HittableList& world) {        
        // file setup
        std::string file_name = "display.ppm";

        // // check if the file already existss
        // if (std::filesystem::exists(file_name)) {
        //     std::cerr << "Error: " << file_name << " already exists\n";
        //     return 1;
        // }

        // open file
        std::ofstream output_file(file_name, std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "Error opening the file\n";
            return 1;
        }

        // PPM header setup
        // The highest value a color channel can have. 256 possible intensities (0 to 255) for each color.
        int max_color_value = 255;

        // Prints the P6 header to signify ppm format 
        output_file << "P6\n"; 
        output_file << cam.get_image_width() << ' ' << cam.get_image_height() << "\n";
        output_file << max_color_value << "\n";

        std::cout << "Setup complete\n"; 

        // Render pixel grid
        for (int row = 0; row < cam.get_image_height(); row++) {
            std::clog << "\rScanlines remaining: " << (cam.get_image_height() - row) << ' ' << std::flush;

            for (int col = 0; col < cam.get_image_width(); col++) {

                Color color_averaged = random_sampling(row, col, sample_amount, cam, world) / sample_amount; 
               
                write_color(output_file, color_averaged);
            }
        }

        std::clog << "\rDone!                 \n";

        // Close the PPM file
        output_file.close();
        return 0;
    }
};

void setup_default_world(HittableList& world) {
    world.add(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f));
}

