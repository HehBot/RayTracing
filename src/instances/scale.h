#ifndef SCALE_H
#define SCALE_H

#include <hittable.h>
#include <memory>
#include <vec3.h>

class aabb;
class ray;

class scale : public hittable {
    std::shared_ptr<hittable> ptr;
    vec3 const m;

public:
    scale(std::shared_ptr<hittable> ptr, vec3 const& m);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box(double time0, double time1) const override;
};

#endif // SCALE_H
