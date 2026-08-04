
#include "render.h"

int main() {
    HittableList world;
    setup_default_world(world);

    Camera cam;
    Renderer ren;
    ren.render(cam, world);
    return 0;
}


