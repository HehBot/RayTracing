#include "sphere.h"

#include <aabb.h>
#include <cmath>
#include <hittable.h>
#include <interval.h>
#include <misc.h>
#include <ray.h>
#include <vec3.h>

class material;

sphere::sphere(pos3 const& position, double radius, std::shared_ptr<material> m)
    : hittable(position), radius(radius), mat_ptr(m), bbox(position - std::fabs(radius) * vec3(1.0, 1.0, 1.0), position + std::fabs(radius) * vec3(1.0, 1.0, 1.0))
{
}

bool sphere::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    vec3 ac = r.origin - position;
    double A = r.direction.length_sq();
    double B_half = dot(r.direction, ac);
    double C = ac.length_sq() - radius * radius;
    double D_quarter = B_half * B_half - A * C;

    if (D_quarter < 0.0)
        return false;

    D_quarter = std::sqrt(D_quarter);

    double root = (-B_half - D_quarter) / A;

    if (!ray_t.contains(root)) {
        root = (-B_half + D_quarter) / A;
        if (!ray_t.contains(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    vec3 outward_normal = (rec.p - position) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

aabb sphere::bounding_box(double time0, double time1) const
{
    return bbox;
}
