#include "rtweekend.hh"

#include "color.hh"
#include "vec3.hh"
#include "ray.hh"
#include "hittable_list.hh"
#include "sphere.hh"
#include "camera.hh"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // The ray can't bounce anymore. It's dissolved into the darkness...
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Reflection point of some rays are not exactly on the surface, but some rays reflect off of
    // slightly inner point of sphere due to floting point error.
    // To workaround this error, ignore the hits that are too close at the origin.
    if (world.hit(r, 0.001, infinity, rec)) {
        // Normalize the target point onto the surface of unit sphere so that the distribution follow
        // Lambert's cosine law, which states that the distribution of diffused ray should be
        // proportional to cos(φ).
        point3 target = rec.p + rec.normal + unit_vector(random_in_unit_sphere());
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
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
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World settings
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // Floating in the air
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // Huge sphere centered at very bottom (the earth)

    camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << std::endl;
    std::cout << 255 << std::endl;

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << "   " << std::flush;

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = double(i + random_double()) / (image_width-1);
                auto v = double(j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << std::endl << "Done" << std::endl;
}