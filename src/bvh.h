#ifndef BVH_H
#define BVH_H

#include <aabb.h>
#include <cstddef>
#include <hittable.h>
#include <memory>
#include <vector>

class hittable_list;
class ray;

class bvh_node : public hittable {
    std::shared_ptr<hittable> left, right;
    aabb box;

public:
    bvh_node() = default;
    bvh_node(std::vector<std::shared_ptr<hittable>> const& objects, std::size_t start, std::size_t end, double time0, double time1);
    bvh_node(hittable_list const& h, double time0, double time1);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box(double time0, double time1) const override;
};

#endif // BVH_H
