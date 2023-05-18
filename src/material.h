#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

struct hit_record;

class material {
public:
    virtual color emitted(double u, double v, pos3 const& p) const
    {
        return color(0.0, 0.0, 0.0);
    }
    virtual bool scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const = 0;
};

#endif // MATERIAL_H
