#include "lambertian.h"

#include "../misc.h"
#include "../ray.h"
#include "../texture.h"
#include "../vec3.h"

#include <memory>

lambertian::lambertian(std::shared_ptr<texture> a)
    : albedo(a)
{
}
bool lambertian::scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const
{
    vec3 scatter_dir = rec.normal + random_unit_vec();
    if (scatter_dir.near_zero())
        scatter_dir = rec.normal;

    scattered = ray(rec.p, scatter_dir, r.time);
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
