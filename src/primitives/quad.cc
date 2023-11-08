#include "quad.h"

#include <aabb.h>
#include <cmath>
#include <constants.h>
#include <cstddef>
#include <hittable.h>
#include <interval.h>
#include <ray.h>
#include <vec3.h>

class material;

quad::quad(pos3 q, vec3 u, vec3 v, std::shared_ptr<material> m)
    : q(q), u(u), v(v), mat_ptr(m)
{
    bbox = aabb(q, q + u + v).pad();

    vec3 n = cross(u, v);
    normal = n.unit_vec();
    d = dot(normal, q);
    w = n / dot(n, n);
}

bool quad::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    double denom = dot(normal, r.direction);
    if (std::fabs(denom) < epsilon)
        return false;

    double t = (d - dot(normal, r.origin)) / denom;
    if (!ray_t.contains(t))
        return false;

    pos3 p = r.at(t);
    vec3 planar_hitpt_vec = p - q;
    double alpha = dot(w, cross(planar_hitpt_vec, v));
    if (alpha < 0.0 || alpha > 1.0)
        return false;
    double beta = dot(w, cross(u, planar_hitpt_vec));
    if (beta < 0.0 || beta > 1.0)
        return false;

    rec.p = p;
    rec.t = t;
    rec.u = alpha;
    rec.v = beta;
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, normal);

    return true;
}

aabb quad::bounding_box(double time0, double time1) const
{
    return bbox;
}
