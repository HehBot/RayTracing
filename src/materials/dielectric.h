#ifndef MATERIALS_DIELECTRIC_H
#define MATERIALS_DIELECTRIC_H

#include "../hittable.h"
#include "../ray.h"
#include "../vec3.h"

#include <memory>

class dielectric : public material {
    static double reflectance(double cos, double ref_ratio);

public:
    double const ir;

    dielectric(double ir);

    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const override;
};

#endif // MATERIALS_DIELECTRIC_H
