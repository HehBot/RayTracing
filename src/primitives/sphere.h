#ifndef PRIMITIVES_SPHERE_H
#define PRIMITIVES_SPHERE_H

#include <hittable.h>
#include <interval.h>
#include <memory>
#include <vec3.h>

class aabb;
class material;
class ray;

class sphere : public hittable {
public:
    double const radius;
    std::shared_ptr<material> const mat_ptr;

    sphere(pos3 const& position, double radius, std::shared_ptr<material> m);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box(double time0, double time1) const override;
};

#endif // PRIMITIVES_SPHERE_H
