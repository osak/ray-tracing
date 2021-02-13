#pragma once

#include "rtweekend.hh"

class camera {
    public:
        // Returns a ray from origin to a point (u, v) in the viewport.
        virtual ray get_ray(double u, double v) const = 0;
};

class ideal_camera : public camera {
    public:
        ideal_camera(
            point3 look_from,
            point3 look_at,
            vec3 vup, // Perpendicular vector to the horizon
            double vfov,
            double aspect_ratio)
        {
            auto theta = deg_to_rad(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h; // y: [-tan(θ/2), tan(θ/2)]
            auto viewport_width = aspect_ratio * viewport_height;

            auto w = unit_vector(look_from - look_at);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            origin = look_from;
            horizontal = viewport_width * u; // Vector equal to a horizontal line from left edge to right
            vertical = viewport_height * v;  // Vector equal to a vertical line from bottom edge to top
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

class lens_camera : public camera {
    public:
        lens_camera(
            point3 look_from,
            point3 look_at,
            vec3 vup, // Perpendicular vector to the horizon
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist)
        {
            auto theta = deg_to_rad(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h; // y: [-tan(θ/2), tan(θ/2)]
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = look_from;
            horizontal = focus_dist * viewport_width * u; // Vector equal to a horizontal line from left edge to right
            vertical = focus_dist * viewport_height * v;  // Vector equal to a vertical line from bottom edge to top
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};