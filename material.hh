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
        metal(const color& a, double f) : albedo(a), fuzz(f) {}

        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }
    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double ir) : ir(ir) {}

        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = unit_vector(r_in.direction());
            auto cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            auto sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            vec3 direction;

            // Assuming a pixel color is sampled with multiple rays, simulate the reflectance by
            // stochastically choosing to reflect or transmit the ray.
            if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double()) {
                // The ray can't be refracted according to Snell's law. It must be reflected.
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double ir; // index of refraction

        // Schlick Approximation of reflectance of a glass
        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine), 5);
        }
};