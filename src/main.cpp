#include <chrono>
#include <iomanip> 

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
    HittableList world;

    Camera cam (16.0f / 9.0f,               // aspect_ratio 
            800,                            // image_width
            20.0f,                          // vfov
            Point3 (13.0f, 2.0f, 3.0f),     // camera_center
            Point3(0,0,0), Vec3(0,1,0),     // lookat
            0.6f,                           // defocus_angle
            10.0f);                         // focus_distance
    
    // samples_per_pixel = 250
    // max_ray_depth = 25
    Renderer ren(250, 25);
    setup_preset_scene(world);

    auto start = std::chrono::steady_clock::now();

    ren.render(cam, world);

    auto end = std::chrono::steady_clock::now();

    auto elapsed = end - start;

    std::cout << "Render Time: "
        << std::fixed << std::setprecision(6)
        << std::chrono::duration<double>(elapsed).count() << " s ("
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms, "
        << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << " us)\n";

    return 0;
}


