#include "color.hh"
#include "vec3.hh"
#include "ray.hh"

#include <iostream>

// Return a real number t, where r.origin + t*r.direction is a point that the ray hits the sphere.
// If the ray hits the sphere at two points (i.e. penetrates the sphere), return the smallest t.
// If the ray doesn't hit the sphere, return -1.0.
// (Negative t is considered invalid as rays won't go backwards)
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        // Norm vector at the hit point
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());

    // Normalize y component of range [-1.0, 1.0] into [0.0, 1.0]
    auto level = 0.5 * (unit_direction.y() + 1.0);

    // (roughly) white in the bottom, sky-blue on the top
    return (1.0-level) * color(1.0, 1.0, 1.0) + level*color(0.5, 0.7, 1.0);
}


int main() {
    // Canvas settings
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera settings
    auto viewport_height = 2.0; // y: [-1.0, 1.0]
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0); // Vector equal to a horizontal line from left edge to right
    auto vertical = vec3(0, viewport_height, 0); // Vector equal to a vertical line from bottom edge to top
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << std::endl;
    std::cout << 255 << std::endl;

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << "   " << std::flush;

        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << std::endl << "Done" << std::endl;
}