
#include "render.h"

void setup_default_world(HittableList& world) {
    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));
}

int main() {
    HittableList world;
    setup_default_world(world);

    Camera cam;
    Renderer ren;
    std::cout << ren.render(cam, world);
    return 0;
}


