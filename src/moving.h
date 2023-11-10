#ifndef MOVING_H
#define MOVING_H

#include <aabb.h>
#include <functional>
#include <hittable.h>
#include <memory>
#include <vec3.h>

class interval;
class ray;

class moving : public hittable {
    std::shared_ptr<hittable> ptr;
    std::function<pos3(double)> path; // position at time t is position + path(t)
    aabb bbox;

public:
    moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box() const override;
};

#endif // MOVING_H
