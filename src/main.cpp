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
    std::ofstream output_file(file_name, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Error opening the file\n";
        return 1;
    }

    // HEADER SETUP
    // dimensions of the image in pixels (default prefixed value for now)
    int width = 256, height = 256;
    // The highest value a color channel can have. 256 possible intensities (0 to 255) for each color.
    int max_color_value = 255;

    // prints the P6 header to signify ppm format 
    output_file << "P6\n"; 
    output_file << width << ' ' << height << "\n";
    output_file << max_color_value << "\n";

    // RENDER PIXEL GRID
    // currently using pre-fixed RBG values to inverse Peter Shirley's example
    for (int row = 0; row < height; row++) {
        std::clog << "\rScanlines remaining: " << (height - row) << ' ' << std::flush;
        for (int col = 0; col < width; col++) {
            auto pixel_color = color(float(row)/(width-1), float(col)/(height-1), 0);
            write_color(output_file, pixel_color);
        }
    }

    std::clog << "\rDone!                 \n";

    // close the file
    output_file.close();
    return 0;
}
 





int main() {
    std::cout << "Ray Tracing Begun" << std::endl;

    std::cout << display_ppm();
    return 0;
}

