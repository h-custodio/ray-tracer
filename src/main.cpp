
#include "render.h"

inline void setup_default_world(HittableList& world) {
    // setup for material coloring
    auto material_ground = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    auto material_center = std::make_shared<Lambertian>(Color(0.1f, 0.2f, 0.5f));
    auto material_left   = std::make_shared<Metal>(Color(0.8f, 0.8f, 0.8f), 0.3f);
    auto material_right  = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 1.0f);

    // make spheres
    world.add(std::make_shared<Sphere>(Point3( 0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0f,    0.0f, -1.2f),   0.5f, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0f,    0.0f, -1.0f),   0.5f, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0f,    0.0f, -1.0f),   0.5f, material_right));
}

int main() {
    HittableList world;
    setup_default_world(world);

    Camera cam;
    Renderer ren;
    ren.render(cam, world);
    return 0;
}


