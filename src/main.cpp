#include <iostream>
#include <string>


#include "camera.h"

int main() {
    std::cout << "Ray Tracing Begun" << std::endl;

    camera cam;
    std::cout << cam.render();
    return 0;
}


