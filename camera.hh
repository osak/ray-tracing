#pragma once

#include "rtweekend.hh"

class camera {
    public:
        camera() {
            const auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0; // y: [-1.0, 1.0]
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