#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <hittable.h>
#include <memory>
#include <vec3.h>

class aabb;
class ray;

class translate : public hittable {
    std::shared_ptr<hittable> ptr;
    vec3 const offset;

public:
    translate(std::shared_ptr<hittable> ptr, vec3 const& offset);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box(double time0, double time1) const override;
};

#endif // TRANSLATE_H
