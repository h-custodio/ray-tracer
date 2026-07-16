#include <iostream>
#include <fstream> 
#include <string>
#include <cmath>

#include "vec3.h"
#include "ray.h"
#include "camera.h"







int main() {
    std::cout << "Ray Tracing Begun" << std::endl;

    camera cam;
    std::cout << cam.render();
    return 0;
}


