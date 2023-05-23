#include "moving.h"

#include "aabb.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <functional>

moving::moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func)
    : hittable(p->position), ptr(p), path(path_func)
{
}

bool moving::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 offset = path(r.time);
    ray moved_r(r.origin - offset, r.direction, r.time);
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
}
// TODO fix this janky implementation
bool moving::bounding_box(double time0, double time1, aabb& output_box) const
{
    aabb initial_box;
    if (!ptr->bounding_box(time0, time1, initial_box))
        return false;
    output_box = initial_box;
    std::size_t constexpr z = 10;
    double d = fmax((time1 - time0) / z, 0.1);
    for (double t = time0; t <= time1; t += d) {
        aabb temp_box(initial_box.min + path(t), initial_box.max + path(t));
        output_box = surrounding_box(output_box, temp_box);
    }
    return true;
}
