#ifndef PRIMITIVES_TRIANGLE_H
#define PRIMITIVES_TRIANGLE_H

#include "../hittable.h"
#include "../material.h"
#include "../ray.h"
#include "../vec3.h"

class triangle : public hittable {
public:
    vec3 const v1, v2, v3, normal;
    std::shared_ptr<material> const mat_ptr;

    triangle(pos3 const& v1, pos3 const& v2, pos3 const& v3, std::shared_ptr<material> m);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // PRIMITIVES_TRIANGLE_H
