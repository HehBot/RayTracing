#include "aarect.h"

#include "../aabb.h"
#include "../hittable.h"
#include "../material.h"
#include "../ray.h"

#include <memory>
#include <stdexcept>

// xy_rect
xy_rect::xy_rect(double x0, double y0, double x1, double y1, double z, std::shared_ptr<material> mat)
    : x0(x0), y0(y0), x1(x1), y1(y1), z(z), mp(mat)
{
    if (x1 <= x0 || y1 <= y0)
        throw std::invalid_argument("Bad coordinates for xy_rect");
}
bool xy_rect::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (z - r.origin.z) / r.direction.z;
    if (t > t_max || t < t_min)
        return false;
    double x = r.origin.x + t * r.direction.x;
    if (x > x1 || x < x0)
        return false;
    double y = r.origin.y + t * r.direction.y;
    if (y > y1 || y < y0)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.set_face_normal(r, vec3(0.0, 0.0, 1.0));
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(pos3(x0, y0, z - 0.0001), pos3(x1, y1, z + 0.0001));
    return true;
}

// yz_rect
yz_rect::yz_rect(double y0, double z0, double y1, double z1, double x, std::shared_ptr<material> mat)
    : y0(y0), z0(z0), y1(y1), z1(z1), x(x), mp(mat)
{
    if (y1 <= y0 || z1 <= z0)
        throw std::invalid_argument("Bad coordinates for yz_rect");
}
bool yz_rect::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (x - r.origin.x) / r.direction.x;
    if (t > t_max || t < t_min)
        return false;
    double y = r.origin.y + t * r.direction.y;
    if (y > y1 || y < y0)
        return false;
    double z = r.origin.z + t * r.direction.z;
    if (z > z1 || z < z0)
        return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.set_face_normal(r, vec3(1.0, 0.0, 0.0));
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
bool yz_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(pos3(x - 0.0001, y0, z0), pos3(x + 0.0001, y1, z1));
    return true;
}

// zx_rect
zx_rect::zx_rect(double z0, double x0, double z1, double x1, double y, std::shared_ptr<material> mat)
    : z0(z0), x0(x0), z1(z1), x1(x1), y(y), mp(mat)
{
    if (z1 <= z0 || x1 <= x0)
        throw std::invalid_argument("Bad coordinates for zx_rect");
}
bool zx_rect::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (y - r.origin.y) / r.direction.y;
    if (t > t_max || t < t_min)
        return false;
    double z = r.origin.z + t * r.direction.z;
    if (z > z1 || z < z0)
        return false;
    double x = r.origin.x + t * r.direction.x;
    if (x > x1 || x < x0)
        return false;

    rec.u = (z - z0) / (z1 - z0);
    rec.v = (x - x0) / (x1 - x0);
    rec.t = t;
    rec.set_face_normal(r, vec3(0.0, 1.0, 0.0));
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
bool zx_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(pos3(x0, y - 0.0001, z0), pos3(x1, y + 0.0001, z1));
    return true;
}
