#include <aabb.h>
#include <algorithm>
#include <bvh.h>
#include <hittable.h>
#include <interval.h>
#include <misc.h>

inline bool compare_helper(std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b, int axis)
{
    auto const& I_a = a->bounding_box().axis(axis);
    auto const& I_b = b->bounding_box().axis(axis);
    return I_a.min < I_b.min;
}

void bvh_node::construct(std::vector<std::shared_ptr<hittable>>& objects, std::size_t start, std::size_t end)
{
    std::size_t objectspan = end - start;

    if (objectspan == 1) {
        left = right = objects[start];
        bbox = left->bounding_box();
    } else if (objectspan == 2) {
        left = objects[start];
        right = objects[start + 1];
        bbox = aabb(left->bounding_box(), right->bounding_box());
    } else {
        int axis = random_int(0, 3);
        auto comparator = [axis](std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b) {
            return compare_helper(a, b, axis);
        };
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectspan / 2;
        std::shared_ptr<bvh_node> tmp = std::make_shared<bvh_node>();
        tmp->construct(objects, start, mid);
        left = tmp;
        tmp = std::make_shared<bvh_node>();
        tmp->construct(objects, mid, end);
        right = tmp;
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }
}
