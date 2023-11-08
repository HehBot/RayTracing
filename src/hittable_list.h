#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <aabb.h>
#include <hittable.h>
#include <interval.h>
#include <memory>
#include <vector>

class ray;

class hittable_list : public hittable {
public:
    hittable_list() = default;
    std::vector<std::shared_ptr<hittable>> list;

    void clear();
    void add(std::shared_ptr<hittable> o);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box(double time0, double time1) const override;
};

#endif // HITTABLE_LIST_H
