#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <vec3.h>

class interval;
class ray;

class translate : public hittable {
    std::shared_ptr<hittable> ptr;
    vec3 const offset;
    aabb bbox;

public:
    translate(std::shared_ptr<hittable> ptr, vec3 const& offset);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box() const override;
};

#endif // TRANSLATE_H
