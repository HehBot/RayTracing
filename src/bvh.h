#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"
#include "vec3.h"

#include <memory>

class bvh_node : public hittable {
    std::shared_ptr<hittable> left, right;
    aabb box;

public:
    bvh_node() = default;
    bvh_node(std::vector<std::shared_ptr<hittable>> const& objects, std::size_t start, std::size_t end, double time0, double time1);
    bvh_node(hittable_list const& h, double time0, double time1);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // BVH_H
