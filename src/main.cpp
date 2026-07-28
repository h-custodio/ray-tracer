
#include "camera.h"

void setup_default_world(hittable_list& world) {
    world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));
}

int main() {
    hittable_list world;
    setup_default_world(world);

    camera cam;
    std::cout << cam.render(world);
    return 0;
}


