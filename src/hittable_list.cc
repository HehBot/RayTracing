#include <aabb.h>
#include <algorithm>
#include <hittable.h>
#include <hittable_list.h>
#include <interval.h>
#include <memory>

class ray;

void hittable_list::add(std::shared_ptr<hittable> o)
{
    list.push_back(o);
    bbox = aabb(bbox, o->bounding_box());
}

bool hittable_list::hit(ray const& r, interval ray_t, hit_record& rec) const
{
    bool any_hit = false;
    double closest_so_far = ray_t.max;

    for (auto const& obj : list) {
        hit_record rec_temp;
        if (obj->hit(r, interval(ray_t.min, closest_so_far), rec_temp)) {
            any_hit = true;
            closest_so_far = rec_temp.t;
            rec = rec_temp;
        }
    }

    return any_hit;
}
aabb hittable_list::bounding_box() const
{
    return bbox;
}
