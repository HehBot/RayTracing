#include "triangle.h"

#include <aabb.h>
#include <cmath>
#include <constants.h>
#include <hittable.h>
#include <interval.h>
#include <ray.h>
#include <vec3.h>

class material;

triangle::triangle(pos3 const& v1, pos3 const& v2, pos3 const& v3, std::shared_ptr<material> m)
    : v1(v1), v2(v2), v3(v3), normal(cross(v2 - v1, v3 - v1)), mat_ptr(m)
{
    bbox = aabb(aabb(v1, v2), aabb(aabb(v2, v3), aabb(v3, v1))).pad();
}

bool triangle::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    double d = dot(normal, r.direction);
    if (std::fabs(d) < epsilon)
        return false;

    double t = dot(normal, v1 - r.origin) / d;
    if (!ray_t.contains(t))
        return false;

    double alpha = dot(r.direction, cross(r.origin - v1, v3 - v1)) / d;
    if (alpha < 0.0)
        return false;
    double beta = dot(r.direction, cross(v2 - v1, r.origin - v1)) / d;
    if (beta < 0.0 || alpha + beta > 1.0)
        return false;

    rec.p = r.at(t);
    rec.t = t;
    rec.u = alpha;
    rec.v = beta;
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, normal);

    return true;
}

aabb triangle::bounding_box() const
{
    return bbox;
}
