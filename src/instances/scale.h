#ifndef SCALE_H
#define SCALE_H

#include "../aabb.h"
#include "../hittable.h"
#include "../ray.h"

#include <memory>

class scale : public hittable {
    std::shared_ptr<hittable> ptr;
    vec3 const m;

public:
    scale(std::shared_ptr<hittable> ptr, vec3 const& m);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // SCALE_H
