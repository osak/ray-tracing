#pragma once

#include "rtweekend.hh"

class camera {
    public:
        camera(double vfov, double aspect_ratio) {
            auto theta = deg_to_rad(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h; // y: [-tan(θ/2), tan(θ/2)]
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = point3(0, 0, 0);
            horizontal = vec3(viewport_width, 0, 0); // Vector equal to a horizontal line from left edge to right
            vertical = vec3(0, viewport_height, 0);  // Vector equal to a vertical line from bottom edge to top
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
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