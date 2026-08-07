#pragma once

#include <filesystem>
#include <fstream> 

#include "vec3.h"
#include "utils.h"

// coordinate system convention: right-handed coordinates
// y-axis goes up 
// x-axis goes right 
// negative z-axis points to the direction we are looking
class Camera {
private: 
    // ========== Explicit Input State ========== //
    
    // defaulted for scene setup ease

    // image properties
    float aspect_ratio = 16.0f / 9.0f;      // ratio of image width over height
    int image_width = 400;

    // positional camera
    float vfov = 20.0f; // vertical view angle (field of view)
    Point3 camera_center = Point3(0.0f, 0.0f, 0.0f);   // point camera is looking from, relative center of screen
    Point3 lookat = Point3(0.0f, 0.0f, -1.0f);         // point camera is looking at
    Vec3 vup = Vec3(0.0f, 1.0f, 0.0f);                 // camera-relative "up" direction
   
    // defcous blur
    float defocus_angle = 0.0f;  // variation angle of rays through each pixel
    float focus_distance = 10.0f;    // distance from camera lookfrom point to plane of perfect focus

    // ========== Derived State ========== //

    // image and camera properties
    int image_height;

    // viewport dimensions
    float viewport_width; 
    float viewport_height;
    Vec3 viewport_horizontal_vector;    // the horizontal top edge of your viewport
    Vec3 viewport_vertical_vector;      // the vertical side edge of your viewport

    // pixel properties
    Vec3 horizontal_pixel_delta;
    Vec3 vertical_pixel_delta;
    Point3 first_pixel_location;    // the very first pixel

    // positional camera
    Vec3 u;     // camera right
    Vec3 v;     // camera up
    Vec3 w;     // camera backward/view axis

    // defocus blur
    Vec3   defocus_disk_horizontal;     // horizontal radius
    Vec3   defocus_disk_vertical;       // vertical radius

    // ========== Functions ========== //

    // configures derived state based on core data
    void configure_camera_state() {
        // image dimensions
        image_height =  static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;   // Prevent 0 height

        // viewport dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        viewport_height = 2 * h * focus_distance;
        viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

        // positional camera
        // Establish camera orientation in 3D space
        w = unit_vector(camera_center - lookat);
        u = unit_vector(cross_product(vup, w));
        v = cross_product(w, u);

        // viewport vectors
        viewport_horizontal_vector = viewport_width * u;    // Vector across viewport horizontal edge
        viewport_vertical_vector = viewport_height * -v;    // Vector down viewport vertical edge

        // pixel spacing
        horizontal_pixel_delta = viewport_horizontal_vector / static_cast<float>(image_width);
        vertical_pixel_delta = viewport_vertical_vector / static_cast<float>(image_height);

        // first pixel location
        auto viewport_upper_left = camera_center - (focus_distance * w)
            - viewport_horizontal_vector / 2.0f 
            - viewport_vertical_vector / 2.0f;

        first_pixel_location = viewport_upper_left + 0.5f * (horizontal_pixel_delta + vertical_pixel_delta);

        // calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_horizontal = u * defocus_radius;
        defocus_disk_vertical = v * defocus_radius;
    }
public:
    // default constructor uses defaulted values
    Camera() {
        configure_camera_state();
    }

    Camera(float aspect_ratio, int image_width,  float vfov, Point3 camera_center, 
        Point3 lookat, Vec3 vup, float defocus_angle, float focus_distance) : 
            aspect_ratio(aspect_ratio),
            image_width(image_width),
            vfov(vfov),
            camera_center(camera_center),
            lookat(lookat),
            vup(vup),
            defocus_angle(defocus_angle),
            focus_distance(focus_distance)
            { 
                configure_camera_state(); 
            }

    // ========== Getters ========== //

    // getters for image and viewport dimensions

    int get_image_width() const { return image_width; }
    int get_image_height() const { return image_height; }

    // getters for vectors and deltas
    Vec3 get_horizontal_pixel_delta() const { return horizontal_pixel_delta; }
    Vec3 get_vertical_pixel_delta() const { return vertical_pixel_delta; }

    // getters for camera properties
    Point3 get_camera_center() const { return camera_center; }     
    Point3 get_first_pixel_location() const { return first_pixel_location; } 

    // getter for defocus blur properties
    float get_defocus_angle() const { return defocus_angle; }
    float get_focus_distance() const { return focus_distance; }

    Vec3 get_defocus_disk_horizontal() const { return defocus_disk_horizontal; }
    Vec3 get_defocus_disk_vertical() const { return defocus_disk_vertical; }

    // ========== Setters ========== //

    void set_camera_center(const Point3& p) { 
        camera_center = p;  
        configure_camera_state();
    }

    void set_vfov(float fov) {
        vfov = fov;
        configure_camera_state();
    }

    void set_lookat(const Point3& p) {
        lookat = p;
        configure_camera_state();
    }
    void set_vup(const Vec3& v) {
        vup = v;
        configure_camera_state();
    }

    void set_defocus_angle(float angle) {
        defocus_angle = angle;
        configure_camera_state();
    }

    void set_focus_distance(float distance) {
        focus_distance = distance;
        configure_camera_state();
    }
};
