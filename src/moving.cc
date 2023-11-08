#include <aabb.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <hittable.h>
#include <moving.h>
#include <ray.h>
#include <vec3.h>

moving::moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func)
    : hittable(p->position), ptr(p), path(path_func)
{
}

bool moving::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    vec3 offset = path(r.time);
    ray moved_r(r.origin - offset, r.direction, r.time);
    if (!ptr->hit(moved_r, ray_t, rec))
        return false;
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
}
// TODO fix this janky implementation
aabb moving::bounding_box(double time0, double time1) const
{
    aabb bbox = ptr->bounding_box(time0, time1);
    std::size_t constexpr z = 10;
    double d = std::fmax((time1 - time0) / z, 0.1);
    for (double t = time0; t <= time1; t += d)
        bbox = aabb(bbox, bbox.offset(path(t)));
    return bbox;
}
