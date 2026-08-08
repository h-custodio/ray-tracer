#include <chrono>
#include <iomanip> 
#include <vector>

#include "render.h"
#include "sphere.h"

inline void setup_preset_scene(HittableList& world) {
    auto ground_material = std::make_shared<Lambertian>(Color(0.5f, 0.5f, 0.5f));
    world.add(make_shared<Sphere>(Point3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = generate_random(0.0f, 1.0f);
            Point3 center(a + 0.9f * generate_random(0.0f, 1.0f), 0.2f, b + 0.9f * generate_random(0.0f, 1.0f));

            if ((center - Point3(4.0f, 0.2f, 0.0f)).magnitude() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = random_vector(0.0f, 1.0f) * random_vector(0.0f, 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = random_vector(0.5f, 1.0f);
                    auto fuzz = generate_random(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5f);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    world.add(std::make_shared<Sphere>(Point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
    world.add(std::make_shared<Sphere>(Point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(std::make_shared<Sphere>(Point3(4.0f, 1.0f, 0.0f), 1.0f, material3));
}

int main() {
    auto program_start = std::chrono::steady_clock::now();

    auto init_start = std::chrono::steady_clock::now();
    std::string file_name = "display.ppm";

    HittableList world;

    Camera cam (16.0f / 9.0f,               // aspect_ratio 
            1200,                            // image_width
            20.0f,                          // vfov
            Point3 (13.0f, 2.0f, 3.0f),     // camera_center
            Point3(0,0,0), Vec3(0,1,0),     // lookat
            0.6f,                           // defocus_angle
            10.0f);                         // focus_distance
    
    // samples_per_pixel = 250
    // max_ray_depth = 25
    Renderer ren(500, 50);

    auto init_end = std::chrono::steady_clock::now();

    auto scene_setup_start = std::chrono::steady_clock::now();
    setup_preset_scene(world);
    auto scene_setup_end = std::chrono::steady_clock::now();

    auto render_start = std::chrono::steady_clock::now();
    auto framebuffer = ren.render(cam, world);
    auto render_end = std::chrono::steady_clock::now();

    auto write_output_start = std::chrono::steady_clock::now();
    write_file(framebuffer, "display.ppm", cam);
    auto write_output_end = std::chrono::steady_clock::now();


    auto program_end = std::chrono::steady_clock::now();

    /// ========== Print Timings ========== //

    auto program_elapsed = program_end - program_start;
    auto init_elapsed = init_end - init_start;
    auto scene_setup_elapsed = scene_setup_end - scene_setup_start;
    auto render_elapsed = render_end - render_start;
    auto write_output_elapsed = write_output_end - write_output_start;

    std::cout << "Initialize Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(init_elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(init_elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(init_elapsed).count() << " us)\n";

    std::cout << "Scene Setup Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(scene_setup_elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(scene_setup_elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(scene_setup_elapsed).count() << " us)\n";

    std::cout << "Render Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(render_elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(render_elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(render_elapsed).count() << " us)\n";

    std::cout << "Write Output Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(write_output_elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(write_output_elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(write_output_elapsed).count() << " us)\n";

    std::cout << "Total Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(program_elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(program_elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(program_elapsed).count() << " us)\n";
    
    /// ========== Runtime Fractions ========== //
    double program_seconds = std::chrono::duration<double>(program_elapsed).count();

    double init_seconds = std::chrono::duration<double>(init_elapsed).count();

    double scene_setup_seconds = std::chrono::duration<double>(scene_setup_elapsed).count();

    double render_seconds = std::chrono::duration<double>(render_elapsed).count();

    double write_output_seconds = std::chrono::duration<double>(write_output_elapsed).count();

    double init_fraction = init_seconds / program_seconds;
    double scene_fraction = scene_setup_seconds / program_seconds;
    double render_fraction = render_seconds / program_seconds;
    double write_fraction = write_output_seconds / program_seconds;

    std::cout << "\n========== Runtime Fractions (Approximate) ==========\n";

    std::cout << "Initialization Fraction: "
            << std::fixed << std::setprecision(6)
            << init_fraction * 100 << "%\n";

    std::cout << "Scene Setup Fraction:    "
            << scene_fraction * 100 << "%\n";

    std::cout << "Render Fraction:         "
            << render_fraction * 100 << "%\n";

    std::cout << "Write Fraction:         "
        << write_fraction * 100 << "%\n";
    
    return 0;
}


