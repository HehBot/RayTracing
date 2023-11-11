#ifndef BVH_H
#define BVH_H

#include <aabb.h>
#include <cstddef>
#include <hittable.h>
#include <hittable_list.h>
#include <interval.h>
#include <memory>
#include <vector>

class ray;

class bvh_node : public hittable {
    std::shared_ptr<hittable> left, right;
    aabb bbox;

    void construct(std::vector<std::shared_ptr<hittable>>& objects, std::size_t start, std::size_t end);

public:
    bvh_node() = default;
    bvh_node(hittable_list h)
    {
        construct(h.list, 0, h.list.size());
    }

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override
    {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right && right->hit(r, (hit_left ? interval(ray_t.min, rec.t) : ray_t), rec);
        return hit_left || hit_right;
    }
    virtual aabb bounding_box() const override
    {
        return bbox;
    }
};

#endif // BVH_H
