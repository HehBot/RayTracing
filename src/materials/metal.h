#ifndef MATERIALS_METAL_H
#define MATERIALS_METAL_H

#include <material.h>
#include <memory>
#include <vec3.h>

class ray;
class texture;
struct hit_record;

class metal : public material {
public:
    std::shared_ptr<texture> albedo;
    double const fuzz;

    metal(std::shared_ptr<texture> a, double f);

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

#endif // MATERIALS_METAL_H
