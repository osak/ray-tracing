#pragma once

#include "ray.hh"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

class hittable {
    public:
        // Returns true if the ray hits this object. The hit point must be between
        // r.origin()+t_min*r.direction() and r.origin()+t_max*r.direction().
        // If it returns true, details of hit point will be stored in rec.
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};