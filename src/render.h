#pragma once

#include "camera.h"

class Renderer {
private:
    Color normal_to_color(const Vec3& unit_vector) {
        return Color((unit_vector.x() + 1) / 2,
            (unit_vector.y() + 1) / 2, 
            (unit_vector.z() + 1) / 2);
    }

    Color ray_color(const Ray& r, const Hittable& world) {
        HitRecord record;

        // if ray hits something in front of camera
        if (world.hit(r, Interval(0, infinity), record)) {
            return  0.5f * (record.normal + Color(1,1,1));
        } 

        auto direction_unit_vector = normalize(r.direction);
        auto a = 0.5f * (direction_unit_vector.y() + 1);

        //                  startValue                    endValue
        return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
    }
public:
    int render(Camera cam, HittableList& world) {        
        // file setup
        std::string file_name = "display.ppm";

        // check if the file already existss
        if (std::filesystem::exists(file_name)) {
            std::cerr << "Error: " << file_name << " already exists\n";
            return 1;
        }

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
                
                // Initialize pixel positioning
                auto pixel_center = cam.get_first_pixel_location() + 
                    (col * cam.get_horizontal_pixel_delta()) + 
                    (row * cam.get_vertical_pixel_delta());
                
                // Initialize ray
                auto ray_direction = pixel_center - cam.get_camera_position();
                Ray r(cam.get_camera_position(), ray_direction);

                Color pixel_color = ray_color(r, world);
                write_color(output_file, pixel_color);
            }
        }

        std::clog << "\rDone!                 \n";

        // Close the PPM file
        output_file.close();
        return 0;
    }
};

void setup_default_world(HittableList& world) {
    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));
}