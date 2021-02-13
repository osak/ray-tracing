#pragma once

#include "hittable.hh"
#include "vec3.hh"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    private:
        point3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Solve a quadratic equation to find a real number `t` where
    // r.origin() + t*r.direction() is on this sphere.
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    // First root
    auto root = (-half_b - sqrtd) / a;

    // If the first root is not within the expected range, try the other one
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
    }

    // If neither root works, the ray is not considered hitting this sphere.
    if (root < t_min || t_max < root) {
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    return true;
}