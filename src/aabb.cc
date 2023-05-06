#include "aabb.h"

#include "ray.h"
#include "vec3.h"

aabb::aabb(pos3 const& a, pos3 const& b)
    : min(a), max(b)
{
}

bool aabb::hit(ray const& r, double t_min, double t_max) const
{
    for (std::size_t i = 0; i < 3; ++i) {
        double invD = 1.0 / r.direction[i];
        double t0 = (min[i] - r.origin[i]) * invD;
        double t1 = (max[i] - r.origin[i]) * invD;

        if (invD < 0)
            std::swap(t0, t1);

        t_min = (t0 < t_min ? t_min : t0);
        t_max = (t1 > t_max ? t_max : t1);

        if (t_min >= t_max)
            return false;
    }
    return true;
}

aabb surrounding_box(aabb const& b1, aabb const& b2)
{
    pos3 min(fmin(b1.min.x, b2.min.x),
             fmin(b1.min.y, b2.min.y),
             fmin(b1.min.z, b2.min.z));
    pos3 max(fmax(b1.max.x, b2.max.x),
             fmax(b1.max.y, b2.max.y),
             fmax(b1.max.z, b2.max.z));
    return aabb(min, max);
}
