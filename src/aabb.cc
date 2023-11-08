#include <aabb.h>
#include <cmath>
#include <cstddef>
#include <ray.h>
#include <utility>
#include <vec3.h>

bool aabb::hit(ray const& r, interval ray_t) const
{
    for (std::size_t i = 0; i < 3; ++i) {
        double invD = 1.0 / r.direction[i];
        double orig = r.origin[i];

        double t0 = (axis(i).min - orig) * invD;
        double t1 = (axis(i).max - orig) * invD;

        if (invD < 0)
            std::swap(t0, t1);

        if (t0 > ray_t.min)
            ray_t.min = t0;
        if (t1 < ray_t.max)
            ray_t.max = t1;

        if (ray_t.min >= ray_t.max)
            return false;
    }
    return true;
}
