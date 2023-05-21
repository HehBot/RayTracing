#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vec3.h"

double constexpr half_min_thickness = 1e-4;

class aabb {
public:
    pos3 min, max;
    aabb() = default;
    aabb(pos3 const& a, pos3 const& b);

    bool hit(ray const& r, double t_min, double t_max) const;
};

aabb surrounding_box(aabb const& b1, aabb const& b2);

#endif // AABB_H
