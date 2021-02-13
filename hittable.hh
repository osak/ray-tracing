#pragma once

#include "ray.hh"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        // Returns true if the ray hits this object. The hit point must be between
        // r.origin()+t_min*r.direction() and r.origin()+t_max*r.direction().
        // If it returns true, details of hit point will be stored in rec.
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};