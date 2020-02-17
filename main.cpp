#include "float.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include <iostream>

inline vec3 normal_to_color(const vec3& vec) {
    return 0.5*vec3(vec.x()+1, vec.y()+1, vec.z()+1);
}

vec3 color(const ray& r, hittable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        // we hit something in the world; get the color which
        // represents the normal
        return normal_to_color(rec.normal);
    } else {
        // return the color of the world backdrop
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    // the output image size
    int nx = 200;
    int ny = 100;

    // the number of samples per pixel (antialiasing)
    int ns = 100;

    // PPM fileheader
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    // make a world which contains all of the circles that we want
    // to render
    hittable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hittable *world = new hittable_list(list, 2);

    // the camera that is viewing the world
    camera cam;

    // the scan directions treat the image as if it's in quadrant
    // 1 of the cartesion plane, rather than quadrant 4 (like
    // typical image/screen coordinates)

    // scan from bottom to top (j)
    for (int j = ny-1; j >= 0; j--) {
        // scan from left to right (i)
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= float(ns);

            // convert the color to PPM int output and print
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
