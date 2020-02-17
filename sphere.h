#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

class sphere: public hittable {
    public:
        vec3 center;
        float radius;
        material *mat_ptr;

        sphere() {}
        sphere(vec3 cen, float r, material *m)
            : center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(
            const ray& r,
            float t_min,
            float t_max,
            hit_record& rec
        ) const;
};

bool sphere::hit(
    const ray& r,
    float t_min,
    float t_max,
    hit_record& rec
) const {
    // find the offset vector of the center of the sphere
    // from the camera
    vec3 oc = r.origin() - center;

    // We represent the intersection of the ray and the sphere
    // shell as a formula--a quadratic formula, where t is the
    // unknown. Then we can find the discriminant of this formula
    // by plugging in its pieces into the quadratic formula.
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;

    // If the discriminant is negative, the ray doesn't touch the
    // sphere at all. If the discriminant is zero, the ray touches
    // the sphere in exactly one spot. If positive, the ray passes
    // through the sphere (hitting the shell in two places).

    if (discriminant <= 0) {
        // it's a miss, or tangent (grazed the edge)
        return false;
    }

    // it's a hit! get the smallest t value available--the one
    // closest to the ray origin.
    float temp = (-b - sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
        rec.t = temp;
        rec.p = r.point_at_parameter(rec.t);
        rec.normal = (rec.p - center) / radius; // calc normal
        rec.mat_ptr = mat_ptr;
        return true;
    }

    // the smallest t value wasn't in the range we want; try the
    // other t value
    temp = (-b + sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
        rec.t = temp;
        rec.p = r.point_at_parameter(rec.t);
        rec.normal = (rec.p - center) / radius;
        rec.mat_ptr = mat_ptr;
        return true;
    }

    // neither of the two values were in the specified range
    return false;
}

#endif