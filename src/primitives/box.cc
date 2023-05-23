#include "box.h"

#include "../material.h"
#include "../vec3.h"
#include "aarect.h"

#include <memory>

box::box(pos3 const& p0, pos3 const& p1, std::shared_ptr<material> mat_ptr)
    : p0(p0), p1(p1)
{
    sides.add(std::make_shared<xy_rect>(p0.x, p0.y, p1.x, p1.y, p0.z, mat_ptr));
    sides.add(std::make_shared<xy_rect>(p0.x, p0.y, p1.x, p1.y, p1.z, mat_ptr));

    sides.add(std::make_shared<yz_rect>(p0.y, p0.z, p1.y, p1.z, p0.x, mat_ptr));
    sides.add(std::make_shared<yz_rect>(p0.y, p0.z, p1.y, p1.z, p1.x, mat_ptr));

    sides.add(std::make_shared<zx_rect>(p0.z, p0.x, p1.z, p1.x, p0.y, mat_ptr));
    sides.add(std::make_shared<zx_rect>(p0.z, p0.x, p1.z, p1.x, p1.y, mat_ptr));
}

bool box::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    return sides.hit(r, t_min, t_max, rec);
}
bool box::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(p0, p1);
    return true;
}
