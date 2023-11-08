#include <aabb.h>
#include <algorithm>
#include <bvh.h>
#include <hittable.h>
#include <hittable_list.h>
#include <iostream>
#include <misc.h>
#include <vec3.h>

class ray;

inline bool compare_helper(std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b, int axis, double time0, double time1)
{
    aabb box_a = a->bounding_box(time0, time1);
    aabb box_b = b->bounding_box(time0, time1);
    return box_a.axis(axis).min < box_b.axis(axis).min;
}

bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>> const& src_objects, std::size_t start, std::size_t end, double time0, double time1)
    : hittable(pos3(0.0, 0.0, 0.0))
{
    auto objects = src_objects;

    int axis = random_int(0, 3);

    auto comparator = [=](std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b) {
        return compare_helper(a, b, axis, time0, time1);
    };

    std::size_t objectspan = end - start;

    if (objectspan == 1) {
        left = right = objects[start];
    } else if (objectspan == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectspan / 2;
        left = std::make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = std::make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left = left->bounding_box(time0, time1);
    aabb box_right = right->bounding_box(time0, time1);
    box = aabb(box_left, box_right);
}

bvh_node::bvh_node(hittable_list const& h, double time0, double time1)
    : bvh_node(h.list, 0, h.list.size(), time0, time1)
{
}

bool bvh_node::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    if (!box.hit(r, ray_t))
        return false;

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right = right->hit(r, (hit_left ? interval(ray_t.min, rec.t) : ray_t), rec);
    return hit_left || hit_right;
}

aabb bvh_node::bounding_box(double time0, double time1) const
{
    return box;
}
