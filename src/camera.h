#pragma once

#include "vec3.h"

// coordinate system convention: right-handed coordinates
// y-axis goes up, 
// x-axis goes right, 
// negative z-axis points to the direction we are looking
class Camera {
private: 
    // Core data (defaulted for scene setup ease)
    // default 16:9 ratio for now
    // int is used for pixels because we need whole pixels
    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    float viewport_height = 1.0f; // have the viewport height maintain the aspect ratio
    float focal_length = 1.0f;
    point3 camera_position = point3(0, 0, 0); // defaulted at relative center, but mathetmatically at (0, 0 , 0)

    // Derived State
    int image_height;
    float viewport_width; 
    vec3 horizontal_vector;
    vec3 vertical_vector;
    vec3 horizontal_pixel_delta;
    vec3 vertical_pixel_delta;

    void configure_camerera_state() {
        image_height =  static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; // Prevent 0 height

        viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

        horizontal_vector = vec3(viewport_width, 0, 0);
        vertical_vector = vec3(0, -viewport_height, 0);

        horizontal_pixel_delta = horizontal_vector / static_cast<float>(image_width);
        vertical_pixel_delta = vertical_vector / static_cast<float>(image_height);
    }
public:
    // Getters for image and viewport dimensions
    float get_aspect_ratio() const { return aspect_ratio; }
    int get_image_width() const { return image_width; }
    int get_image_height() const { return image_height; }

    float get_viewport_height() const { return viewport_height; }
    float get_viewport_width() const { return viewport_width; }

    // Getters for vectors and deltas
    vec3 get_horizontal_vector() const { return horizontal_vector; }
    vec3 get_vertical_vector() const { return vertical_vector; }
    vec3 get_horizontal_pixel_delta() const { return horizontal_pixel_delta; }
    vec3 get_vertical_pixel_delta() const { return vertical_pixel_delta; }

    // Getters for camera properties
    point3 get_camera_position() const { return camera_position; }
    float get_focal_length() const { return focal_length; }        

    // Setters for image and viewport dimensions
    void set_aspect_ratio(float width, float height) { 
        if (height == 0.0f) {
            throw std::invalid_argument("division by zero");
        }

        aspect_ratio = width / height; 
        configure_camerera_state(); 
    }

    void set_image_width(int width) { 
        image_width = width; 
        configure_camerera_state(); 
    }
    
    void set_viewport_height(float height) { 
        viewport_height = height; 
        configure_camerera_state(); 
    }

    void set_camera_position(const point3& position) { 
        camera_position = position; 
        configure_camerera_state(); 
    }

    void set_focal_length(float length) { 
        focal_length = length; 
        configure_camerera_state(); 
    }
};