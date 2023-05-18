#ifndef PRIMITIVES_AARECT_H
#define PRIMITIVES_AARECT_H

#include "../aabb.h"
#include "../hittable.h"
#include "../material.h"
#include "../ray.h"

#include <memory>

class xy_rect : public hittable {
    double const x0, y0, x1, y1, z;
    std::shared_ptr<material> mp;

public:
    xy_rect(double x0, double y0, double x1, double y1, double z, std::shared_ptr<material> mat);
    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

class yz_rect : public hittable {
    double const y0, z0, y1, z1, x;
    std::shared_ptr<material> mp;

public:
    yz_rect(double y0, double z0, double y1, double z1, double x, std::shared_ptr<material> mat);
    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

class zx_rect : public hittable {
    double const z0, x0, z1, x1, y;
    std::shared_ptr<material> mp;

public:
    zx_rect(double z0, double x0, double z1, double x1, double y, std::shared_ptr<material> mat);
    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // PRIMITIVES_AARECT_H
