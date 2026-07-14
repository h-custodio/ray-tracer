#pragma once

#include "vec3.h"

// coordinate system convention: right-handed coordinates
// y-axis goes up, 
// x-axis goes right, 
// negative z-axis points to the direction we are looking
class camera {
private: 
    // Core data (defaulted for scene setup ease)
    // default 16:9 ratio for now
    // int is used for pixels because we need whole pixels
    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    float viewport_height = 2.0f; // have the viewport height maintain the aspect ratio
    float focal_length = 1.0f;
    point3 camera_position = point3(0, 0, 0); // defaulted at relative center, but mathetmatically at (0, 0 , 0)

    // Derived State
    int image_height;
    float viewport_width; 
    vec3 viewport_horizontal_vector; // represents the horizontal top edge of your viewport
    vec3 viewport_vertical_vector; // represents the vertical side edge of your viewport
    vec3 horizontal_pixel_delta;
    vec3 vertical_pixel_delta;
    point3 first_pixel_location; // represents the very first pixel or position (0, 0) aka top left

    void configure_camera_state() {
        image_height =  static_cast<int>(image_width / aspect_ratio);
         // Prevent 0 height
        if (image_height < 1) {
            image_height = 1;
        }

        viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

        viewport_horizontal_vector = vec3(viewport_width, 0, 0);
        viewport_vertical_vector = vec3(0, -viewport_height, 0);

        horizontal_pixel_delta = viewport_horizontal_vector / static_cast<float>(image_width);
        vertical_pixel_delta = viewport_vertical_vector / static_cast<float>(image_height);

        auto viewport_upper_left = camera_position - vec3(0, 0, focal_length)
        - viewport_horizontal_vector / 2 - viewport_vertical_vector / 2;

        first_pixel_location = viewport_upper_left + 0.5 * (horizontal_pixel_delta + vertical_pixel_delta);
    }
public:
    camera() {
        configure_camera_state();
    }

    camera(float a_ratio, int img_width, float vp_height, float f_length, point3 cam_position) {
        aspect_ratio = a_ratio;
        image_width = img_width;
        viewport_height = vp_height;
        focal_length = f_length;
        camera_position = cam_position;
        configure_camera_state();
    }

    // Getters for image and viewport dimensions
    float get_aspect_ratio() const { return aspect_ratio; }
    int get_image_width() const { return image_width; }
    int get_image_height() const { return image_height; }

    float get_viewport_height() const { return viewport_height; }
    float get_viewport_width() const { return viewport_width; }

    // Getters for vectors and deltas
    vec3 get_horizontal_vector() const { return viewport_horizontal_vector; }
    vec3 get_vertical_vector() const { return viewport_vertical_vector; }
    vec3 get_horizontal_pixel_delta() const { return horizontal_pixel_delta; }
    vec3 get_vertical_pixel_delta() const { return vertical_pixel_delta; }

    // Getters for camera properties
    point3 get_camera_position() const { return camera_position; }
    float get_focal_length() const { return focal_length; }        
    point3 get_first_pixel_location() const { return first_pixel_location; } 

    // Setters for image and viewport dimensions
    void set_aspect_ratio(float a_ratio) { 
        aspect_ratio = a_ratio;
        configure_camera_state(); 
    }

    void set_image_width(int img_width) { 
        image_width = img_width; 
        configure_camera_state(); 
    }
    
    void set_viewport_height(float vp_height) { 
        viewport_height = vp_height; 
        configure_camera_state(); 
    }

    void set_focal_length(float f_length) { 
        focal_length = f_length; 
        configure_camera_state(); 
    }

    void set_camera_position(const point3& cam_position) { 
        camera_position = cam_position; 
        configure_camera_state(); 
    }

};

