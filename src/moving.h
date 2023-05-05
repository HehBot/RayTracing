#ifndef MOVING_H
#define MOVING_H

#include "vec3.h"

#include <functional>
#include <memory>

class moving : public hittable {
    std::shared_ptr<hittable> ptr;
    std::function<pos3(double)> path; // position at time t is position + path(t)

public:
    moving(std::shared_ptr<hittable> p, std::function<pos3(double)> path_func)
        : hittable(p->position), ptr(p), path(path_func)
    {
    }

    bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override
    {
        vec3 offset = path(r.time);
        ray moved_r(r.origin - offset, r.direction, r.time);
        if (!ptr->hit(moved_r, t_min, t_max, rec))
            return false;
        rec.p += offset;
        rec.set_face_normal(moved_r, rec.normal);
        return true;
    }
};

#endif // MOVING_H
