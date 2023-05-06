#ifndef MOVING_H
#define MOVING_H

#include "aabb.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <functional>
#include <memory>

class moving : public hittable {
    std::shared_ptr<hittable> ptr;
    std::function<pos3(double)> path; // position at time t is position + path(t)

public:
    moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // MOVING_H
