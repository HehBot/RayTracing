#ifndef MATERIALS_DIFFUSE_LIGHT_H
#define MATERIALS_DIFFUSE_LIGHT_H

#include <material.h>
#include <memory>
#include <vec3.h>

class ray;
class texture;
struct hit_record;

class diffuse_light : public material {
    std::shared_ptr<texture> emit;

public:
    diffuse_light(std::shared_ptr<texture> a);

    virtual color emitted(double u, double v, pos3 const& p) const override;
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

#endif // MATERIALS_DIFFUSE_LIGHT_H
