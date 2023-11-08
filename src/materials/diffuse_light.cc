#include "diffuse_light.h"

#include <memory>
#include <texture.h>
#include <vec3.h>

class ray;
struct hit_record;

diffuse_light::diffuse_light(std::shared_ptr<texture> a)
    : emit(a)
{
}
color diffuse_light::emitted(double u, double v, pos3 const& p) const
{
    return emit->value(u, v, p);
}
bool diffuse_light::scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const
{
    return false;
}
