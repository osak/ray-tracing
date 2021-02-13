#pragma once

#include "rtweekend.hh"
#include "hittable.hh"

class material {
    public:
        // Return true if the incoming ray, which is hitting the object as described in rec,
        // is scattered by this object. Otherwise the ray is completely absorved.
        // If this method returns true, it also reports how much the ray should be attenuated
        // and to which direction the ray should be scattered.
        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            // Normalize the target point onto the surface of unit sphere so that the distribution follow
            // Lambert's cosine law, which states that the distribution of diffused ray should be
            // proportional to cos(φ).
            auto scatter_direction = rec.normal + unit_vector(random_in_unit_sphere());

            // Degenarated case: fall back to the normal vector
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }
    private:
        color albedo;
};