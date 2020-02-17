#include "float.h"
#include "hittable_list.h"
#include "sphere.h"
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

    // PPM fileheader
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    vec3 lower_left_corner(-2.0, -1.0, -1.0); // scan origin
    vec3 horizontal(4.0, 0.0, 0.0); // scan x range
    vec3 vertical(0.0, 2.0, 0.0); // scan y range
    vec3 origin(0.0, 0.0, 0.0);

    hittable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hittable *world = new hittable_list(list, 2);

    // the scan directions treat the image as if it's in quadrant
    // 1 of the cartesion plane, rather than quadrant 4 (like
    // typical image/screen coordinates)

    // scan from bottom to top (j)
    for (int j = ny-1; j >= 0; j--) {
        // scan from left to right (i)
        for (int i = 0; i < nx; i++) {
            // find the proportion across the image that we are,
            // in the range [0..1]
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            // make a ray shooting from this point
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            // find the color that this ray sees
            // vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r, world);

            // convert the color to PPM int output and print
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
