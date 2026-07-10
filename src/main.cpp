#include <iostream>
#include <fstream> 
#include <string>
#include <filesystem>

#include "vec3.h"
#include "color.h"
#include "ray.h"


auto calculate_aspect_ratio(float width, float height) {
    if (height == 0.0f) {
        throw std::invalid_argument("division by zero");
    }

    return width / height;
}

color ray_color(const ray& r) {
    return color();
}

int display_ppm(){
    //FILE SETUP
    std::string file_name = "display.ppm";

    // check if we file already existss
    if (std::filesystem::exists(file_name)) {
        std::cerr << "Error: " << file_name << " already exists\n";
        return 1;
    }

    // tries to open file
    std::ofstream output_file(file_name);
    if (!output_file.is_open()) {
        std::cerr << "Error opening the file\n";
        return 1;
    }

    // HEADER SETUP
    // dimensions of the image in pixels (default prefixed value for now)
    int width = 256, height = 256;
    // The highest value a color channel can have. 256 possible intensities (0 to 255) for each color.
    int max_color_value = 255;

    // prints the P3 header to signify ppm format 
    output_file << "P3\n"; 
    output_file << width << ' ' << height << "\n";
    output_file << max_color_value << "\n";

    // RENDER PIXEL GRID
    // currently using pre-fixed RBG values to inverse Peter Shirley's example
    for (int row = 0; row < height; row++) {
        std::clog << "\rScanlines remaining: " << (height - row) << ' ' << std::flush;
        for (int col = 0; col < width; col++) {
            auto pixel_color = color(double(row)/(width-1), double(col)/(height-1), 0);
            write_color(output_file, pixel_color);
        }
        output_file << "\n";
    }

    std::clog << "\rDone!                 \n";

    // close the file
    output_file.close();
    return 0;
}
 


int main() {
    std::cout << "Ray Tracing Begun" << std::endl;

    // default 16:9 ratio for now
    int aspect_ratio = calculate_aspect_ratio(16.0f, 9.0f);
    int image_width = 400;
    int image_height = image_height / aspect_ratio;

    if (image_height < 1) {
        image_height = 1;
    }

    auto viewport_height = 1.0;
    auto viewport_width = viewport_height * (float(image_width)/image_height);

    // convention: right-handed coordinates
    // y-axis goes up, 
    // x-axis goes right, 
    // negative z-axis points to viewing direction
    auto camera_center = point3();

    auto horizontal_vector = vec3(viewport_width, 0, 0);
    auto vertical_vector  =vec3(0, -viewport_height, 0);

    auto horizontal_pixel_delta = horizontal_vector / image_width; 
    auto vertical_pixel_delta = vertical_vector / image_height;

    std::cout << display_ppm();
    return 0;
}

