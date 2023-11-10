#include <aabb.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <hittable.h>
#include <interval.h>
#include <moving.h>
#include <ray.h>
#include <vec3.h>

moving::moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func)
    : ptr(p), path(path_func)
{
    bbox = ptr->bounding_box();
    std::size_t constexpr z = 10;
    double d = std::fmax((1.0 - 0.0) / z, 0.1);
    for (double t = 0.0; t <= 1.0; t += d)
        bbox = aabb(bbox, bbox.offset(path(t)));
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
aabb moving::bounding_box() const
{
    return bbox;
}
