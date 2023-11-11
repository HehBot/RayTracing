#include <aabb.h>
#include <algorithm>
#include <bvh.h>
#include <hittable.h>
#include <interval.h>
#include <misc.h>

#define SAH_BVH

inline bool compare_helper(std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b, int axis)
{
    auto const& I_a = a->bounding_box().axis(axis);
    auto const& I_b = b->bounding_box().axis(axis);
    return I_a.min < I_b.min;
}

#ifdef BVH
// bounding volume hierarchy with median split along random axis
void bvh_node::construct(std::vector<std::shared_ptr<hittable>>& objects, std::size_t start, std::size_t end)
{
    std::size_t nr_objects = end - start;

    if (nr_objects == 1) {
        left = objects[start];
        right = nullptr;
        bbox = left->bounding_box();
    } else if (nr_objects == 2) {
        left = objects[start];
        right = objects[start + 1];
        bbox = aabb(left->bounding_box(), right->bounding_box());
    } else {
        int axis = random_int(0, 3);
        auto comparator = [axis](std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b) {
            return compare_helper(a, b, axis);
        };
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + nr_objects / 2;
        std::shared_ptr<bvh_node> tmp = std::make_shared<bvh_node>();
        tmp->construct(objects, start, mid);
        left = tmp;
        tmp = std::make_shared<bvh_node>();
        tmp->construct(objects, mid, end);
        right = tmp;
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }
}
#endif // BVH

#ifdef SAH_BVH
// bounding volume hierarchy with SAH split
inline double sah(double sa1, int n1, double sa2, int n2, double sa)
{
    return ((sa1 * n1 + sa2 * n2) / sa);
}
void bvh_node::construct(std::vector<std::shared_ptr<hittable>>& objects, std::size_t start, std::size_t end)
{
    std::size_t nr_objects = end - start;

    if (nr_objects == 1) {
        left = objects[start];
        right = nullptr;
        bbox = left->bounding_box();
    } else if (nr_objects == 2) {
        left = objects[start];
        right = objects[start + 1];
        bbox = aabb(left->bounding_box(), right->bounding_box());
    } else {
        double min_cost = infinity;
        std::size_t min_mid = 0;
        std::vector<std::shared_ptr<hittable>> min_objects;
        min_objects.reserve(nr_objects);
        for (int axis = 0; axis < 3; ++axis) {
            auto comparator = [=](std::shared_ptr<hittable> const& a, std::shared_ptr<hittable> const& b) {
                return compare_helper(a, b, axis);
            };
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            std::vector<double> left_accumulated_bbox_sa(nr_objects), right_accumulated_bbox_sa(nr_objects);
            aabb temp_l, temp_r;
            for (std::size_t i = 0; i < nr_objects; ++i) {
                temp_l = aabb(temp_l, objects[i + start]->bounding_box());
                temp_r = aabb(temp_r, objects[end - i - 1]->bounding_box());
                left_accumulated_bbox_sa[i] = temp_l.surface_area();
                right_accumulated_bbox_sa[i] = temp_r.surface_area();
            }
            bbox = temp_l;

            double bbox_sa = bbox.surface_area();
            bool this_axis_set = false;
            for (std::size_t m = 1; m < nr_objects; ++m) {
                double cost = sah(left_accumulated_bbox_sa[m - 1], m, right_accumulated_bbox_sa[nr_objects - m - 1], nr_objects - m, bbox_sa);
                if (min_cost > cost) {
                    min_mid = m;
                    min_cost = cost;
                    this_axis_set = true;
                }
            }
            if (this_axis_set)
                min_objects = std::vector(objects.begin() + start, objects.begin() + end);
        }

        std::shared_ptr<bvh_node> tmp = std::make_shared<bvh_node>();
        tmp->construct(min_objects, 0, min_mid);
        left = tmp;
        tmp = std::make_shared<bvh_node>();
        tmp->construct(min_objects, min_mid, nr_objects);
        right = tmp;
    }
}
#endif // SAH_BVH
