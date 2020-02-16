#include "ray.h"
#include <iostream>

bool hit_sphere(const vec3& center, float radius, const ray& r) {
    // find the offset vector of the center of the sphere
    // from the camera
    vec3 oc = r.origin() - center;

    // We represent the intersection of the ray and the sphere
    // shell as a formula--a quadratic formula, where t is the
    // unknown. Then we can find the discriminant of this formula
    // by plugging in its pieces into the quadratic formula.
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;

    // We don't actually care what the intersection points are--
    // we just care how many of them there are. The discriminant
    // tells us that. If the discriminant less than zero, the ray
    // doesn't intersect the circle. If it's exactly zero, the ray
    // is tangent to the circle (intersects exactly once). If it's
    // greater than zero, the ray passes through the circle. For
    // the purpose of this function, we only care if we hit the
    // circle or not.
    return (discriminant > 0);
}

vec3 color(const ray& r) {
    if (hit_sphere(vec3(0, 0, -0.5), 0.5, r)) {
        return vec3(1, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
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
            vec3 col = color(r);

            // convert the color to PPM int output and print
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
