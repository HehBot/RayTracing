#include "scale.h"

#include "../aabb.h"
#include "../constants.h"
#include "../hittable.h"
#include "../ray.h"

#include <cmath>
#include <memory>

scale::scale(std::shared_ptr<hittable> ptr, vec3 const& m)
    : ptr(ptr), m(m)
{
    for (std::size_t i = 0; i < 3; ++i)
        if (m[i] < epsilon && m[i] > -epsilon)
            throw std::invalid_argument("Invalid scaling transformation");
}
bool scale::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    ray scaled_r(r.origin / m, r.direction / m, r.time);
    if (!ptr->hit(scaled_r, t_min, t_max, rec))
        return false;

    rec.p *= m;
    rec.normal /= m;
    rec.set_face_normal(r, rec.normal);
    return true;
}
bool scale::bounding_box(double time0, double time1, aabb& output_box) const
{
    aabb bbox;
    if (!ptr->bounding_box(time0, time1, bbox))
        return false;

    for (std::size_t i = 0; i < 3; ++i) {
        output_box.min[i] = bbox.min[i] * m[i];
        output_box.max[i] = bbox.max[i] * m[i];
        if (output_box.min[i] > output_box.max[i])
            std::swap(output_box.min[i], output_box.max[i]);
    }
    return true;
}
