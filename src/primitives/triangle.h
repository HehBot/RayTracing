#ifndef PRIMITIVES_TRIANGLE_H
#define PRIMITIVES_TRIANGLE_H

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <vec3.h>

class interval;
class material;
class ray;

class triangle : public hittable {
    vec3 const v1, v2, v3, normal;
    std::shared_ptr<material> const mat_ptr;
    aabb bbox;

public:
    triangle(pos3 const& v1, pos3 const& v2, pos3 const& v3, std::shared_ptr<material> m);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box() const override;
};

#endif // PRIMITIVES_TRIANGLE_H
