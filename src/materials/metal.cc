#include "metal.h"

#include <hittable.h>
#include <memory>
#include <misc.h>
#include <ray.h>
#include <texture.h>
#include <vec3.h>

metal::metal(std::shared_ptr<texture> a, double f)
    : albedo(a), fuzz(f > 1.0 ? 1.0 : f)
{
}
bool metal::scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const
{
    vec3 reflected = reflect(r.direction, rec.normal).unit_vec() + fuzz * random_vec_in_sphere();
    scattered = ray(rec.p, reflected, r.time);
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return (dot(scattered.direction, rec.normal) > 0.0);
}
