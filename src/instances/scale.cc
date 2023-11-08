#include "scale.h"

#include <aabb.h>
#include <constants.h>
#include <cstddef>
#include <hittable.h>
#include <memory>
#include <ray.h>
#include <stdexcept>
#include <utility>
#include <vec3.h>

scale::scale(std::shared_ptr<hittable> ptr, vec3 const& m)
    : ptr(ptr), m(m)
{
    for (std::size_t i = 0; i < 3; ++i)
        if (m[i] < epsilon && m[i] > -epsilon)
            throw std::invalid_argument("Invalid scaling transformation");
}
bool scale::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    ray scaled_r(r.origin / m, r.direction / m, r.time);
    if (!ptr->hit(scaled_r, ray_t, rec))
        return false;

    rec.p *= m;
    rec.normal /= m;
    rec.set_face_normal(r, rec.normal);
    return true;
}
aabb scale::bounding_box(double time0, double time1) const
{
    return ptr->bounding_box(time0, time1).scale(m);
}
