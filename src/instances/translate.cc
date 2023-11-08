#include "translate.h"

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <ray.h>
#include <vec3.h>

translate::translate(std::shared_ptr<hittable> p, vec3 const& displacement)
    : ptr(p), offset(displacement)
{
}
bool translate::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    ray moved_r(r.origin - offset, r.direction, r.time);
    if (!ptr->hit(moved_r, ray_t, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(r, rec.normal);
    return true;
}
aabb translate::bounding_box(double time0, double time1) const
{
    return ptr->bounding_box(time0, time1).offset(offset);
}
