#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <vector>

class interval;
class ray;

class hittable_list : public hittable {
    std::vector<std::shared_ptr<hittable>> list;
    aabb bbox;

public:
    hittable_list() = default;
    hittable_list(std::vector<std::shared_ptr<hittable>> const& list);

    void add(std::shared_ptr<hittable> o);

    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override;
    virtual aabb bounding_box() const override;

    friend class bvh_node;
};

#endif // HITTABLE_LIST_H
