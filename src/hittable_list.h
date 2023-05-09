#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "aabb.h"
#include "hittable.h"

#include <climits>
#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    hittable_list() = default;
    std::vector<std::shared_ptr<hittable>> list;

    void clear();
    void add(std::shared_ptr<hittable> o);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // HITTABLE_LIST_H
