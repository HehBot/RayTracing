#ifndef MATERIALS_LAMBERTIAN_H
#define MATERIALS_LAMBERTIAN_H

#include "../hittable.h"
#include "../ray.h"
#include "../texture.h"
#include "../vec3.h"

#include <memory>

class lambertian : public material {
public:
    std::shared_ptr<texture> albedo;

    lambertian(std::shared_ptr<texture> a);

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

#endif // MATERIALS_LAMBERTIAN_H
