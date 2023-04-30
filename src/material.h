#ifndef MATERIAL_H
#define MATERIAL_H

#include "misc.h"

#include <iostream>

struct hit_record;

// Abstract class for materials
class material {
public:
    // Returns true if there is a scattered ray emanating from this material type
    // attenuation and scattered are set appropriately
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const = 0;
};

// Lambertian material
class lambertian : public material {
public:
    color const albedo;

    lambertian(color a)
        : albedo(a)
    {
    }

    // Cosine distribution of scattered rays, see https://en.wikipedia.org/wiki/Lambert%27s_cosine_law
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override
    {
        vec3 scatter_dir = rec.normal + random_unit_vec();
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;

        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }
};

// Metallic material
class metal : public material {
public:
    color const albedo;
    double const fuzz;

    metal(color const& a, double f)
        : albedo(a), fuzz(f > 1.0 ? 1.0 : f)
    {
    }

    // Only scattered ray is the reflected ray
    // Reflected ray perturbed according to fuzz for imperfect reflection
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override
    {
        vec3 reflected = reflect(r.direction, rec.normal).unit_vec() + fuzz * random_vec_in_sphere();
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0.0);
    }
};

class dielectric : public material {
public:
    double const ir;

    dielectric(double ir)
        : ir(ir)
    {
    }

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double mu_i_by_mu_t = (rec.front_face ? (1.0 / ir) : ir);

        vec3 unit_incident = r.direction.unit_vec();
        double cos_theta = fmin(dot(-unit_incident, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        vec3 direction;
        if (mu_i_by_mu_t * sin_theta > 1.0 || reflectance(cos_theta, mu_i_by_mu_t) > random_double())
            direction = reflect(unit_incident, rec.normal);
        else
            direction = refract(unit_incident, rec.normal, mu_i_by_mu_t);

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    static double reflectance(double cos, double ref_ratio)
    {
        double r0 = (1 - ref_ratio) / (1 + ref_ratio);
        r0 *= r0;
        return r0 + (1 - r0) * std::pow(1 - cos, 5);
    }
};

#endif /* MATERIAL_H */
