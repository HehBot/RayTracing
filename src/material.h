#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "misc.h"
#include "ray.h"
#include "texture.h"

#include <iostream>
#include <memory>

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
    std::shared_ptr<texture> albedo;

    lambertian(std::shared_ptr<texture> a);

    // Cosine distribution of scattered rays, see https://en.wikipedia.org/wiki/Lambert%27s_cosine_law
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

// Metallic material
class metal : public material {
public:
    std::shared_ptr<texture> albedo;
    double const fuzz;

    metal(std::shared_ptr<texture> a, double f);

    // Only scattered ray is the reflected ray
    // Reflected ray perturbed according to fuzz for imperfect reflection
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

// Dielectric material
class dielectric : public material {
public:
    double const ir;

    dielectric(double ir);

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;

private:
    static double reflectance(double cos, double ref_ratio);
};

#endif /* MATERIAL_H */
