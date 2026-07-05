#include <iostream>
#include <fstream> 
#include <string>
#include <filesystem>
#include <print>
using std::cout;
using std::string;


int display_ppm(){
    
    string file_name = "display.ppm";

    // check if we file already exists
    // if (std::filesystem::exists(file_name)) {
    //     std::cerr << "Error: " << file_name << " already exists\n";
    //     return 1;
    // }

    // tries to open file
    std::ofstream output_file(file_name);

    if (!output_file.is_open()) {
        std::cerr << "Error opening the file\n";
        return 1;
    }

    // dimensions of the image in pixels (default prefixed value for now)
    int width = 256, height = 256;
    // The highest value a color channel can have. 256 possible intensities (0 to 255) for each color.
    int max_color_value = 255;

    // prints the P3 header to signify ppm format 
    output_file << "P3\n"; 
    output_file << width << ' ' << height << "\n";
    output_file << max_color_value << "\n";

    // pixel grid (RGB format)
    // currently using pre-fixed RBG values to inverse Peter Shirley's example
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {
            // normalize current position into a value between 0.0 - 1.0 
            double norm_red = (double)row / max_color_value;
            double norm_green = (double)col / max_color_value;
            double norm_blue = 0;

            //convert normalized into color value
            int red = int(255.99 * norm_red);
            int green = int(255.99 * norm_green);
            int blue = int(255.99 * norm_blue);

            output_file << red << ' ' <<  green << ' ' << blue << "\n";
        }
        output_file << "\n";
    }

    // close the file
    output_file.close();
    return 0;
}


int main() {
    cout << "Ray Tracing Begun" << std::endl;
    
    cout << display_ppm();
    return 0;
}

