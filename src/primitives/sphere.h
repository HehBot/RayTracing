#ifndef PRIMITIVES_SPHERE_H
#define PRIMITIVES_SPHERE_H

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <vec3.h>

class interval;
class material;
class ray;

class sphere : public hittable {
    pos3 position;
    double const radius;
    std::shared_ptr<material> const mat_ptr;
    aabb bbox;

public:
    sphere(pos3 const& position, double radius, std::shared_ptr<material> m);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box() const override;
};

#endif // PRIMITIVES_SPHERE_H
