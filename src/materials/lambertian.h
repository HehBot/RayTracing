#ifndef MATERIALS_LAMBERTIAN_H
#define MATERIALS_LAMBERTIAN_H

#include <material.h>
#include <memory>
#include <vec3.h>

struct hit_record;
class ray;
class texture;

class lambertian : public material {
public:
    std::shared_ptr<texture> albedo;

    lambertian(std::shared_ptr<texture> a);

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

#endif // MATERIALS_LAMBERTIAN_H
