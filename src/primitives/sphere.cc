#include "sphere.h"

#include "../hittable.h"
#include "../misc.h"
#include "../ray.h"
#include "../vec3.h"

void sphere::get_sphere_uv(pos3 const& p, double& u, double& v)
{
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + pi;

    u = 0.5 * phi / pi;
    v = theta / pi;
}

sphere::sphere(pos3 const& position, double radius, std::shared_ptr<material> m)
    : hittable(position), radius(radius), mat_ptr(m)
{
}

bool sphere::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
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

    if (root > t_max || root < t_min) {
        root = (-B_half + D_quarter) / A;
        if (root > t_max || root < t_min)
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

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(position - fabs(radius) * vec3(1.0, 1.0, 1.0), position + fabs(radius) * vec3(1.0, 1.0, 1.0));
    return true;
}
