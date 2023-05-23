#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "../aabb.h"
#include "../hittable.h"
#include "../ray.h"
#include "../vec3.h"

#include <memory>

class translate : public hittable {
    std::shared_ptr<hittable> ptr;
    vec3 const offset;

public:
    translate(std::shared_ptr<hittable> ptr, vec3 const& offset);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // TRANSLATE_H
