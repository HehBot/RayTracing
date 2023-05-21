#include "triangle.h"

#include "../hittable.h"
#include "../misc.h"
#include "../ray.h"
#include "../vec3.h"

triangle::triangle(pos3 const& v1, pos3 const& v2, pos3 const& v3, std::shared_ptr<material> m)
    : v1(v1), v2(v2), v3(v3), normal(cross(v2 - v1, v3 - v1)), mat_ptr(m)
{
}

bool triangle::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    double d = dot(normal, r.direction);

    if (fabs(d) < epsilon)
        return false;
    double t = dot(normal, v1 - r.origin) / d;
    if (t > t_max || t < t_min)
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

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const
{
    for (std::size_t i = 0; i < 3; ++i) {
        double m = v1[i], M = v1[i];
        if (m > v2[i])
            m = v2[i];
        if (m > v3[i])
            m = v3[i];
        if (M < v2[i])
            M = v2[i];
        if (M < v3[i])
            M = v3[i];
        if ((M - m) < 2 * half_min_thickness) {
            M = m = (M + m) / 2;
            m -= half_min_thickness;
            M += half_min_thickness;
        }
        output_box.min[i] = m;
        output_box.max[i] = M;
    }
    return true;
}
