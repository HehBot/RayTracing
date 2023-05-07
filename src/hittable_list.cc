#include "hittable_list.h"

#include "aabb.h"
#include "ray.h"
#include "vec3.h"

#include <memory>

void hittable_list::clear()
{
    list.clear();
}
void hittable_list::add(std::shared_ptr<hittable> o)
{
    list.push_back(o);
}

bool hittable_list::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    bool any_hit = false;
    rec.t = t_max;

    for (auto const& p : list) {
        hit_record rec_temp;
        if (p->hit(r, t_min, rec.t, rec_temp)) {
            any_hit = true;
            rec = rec_temp;
        }
    }

    return any_hit;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
    if (list.empty())
        return false;

    aabb temp_box;
    bool first = true;

    for (auto const& o : list) {
        if (!o->bounding_box(time0, time1, temp_box))
            return false;
        output_box = (first ? temp_box : surrounding_box(output_box, temp_box));
        first = false;
    }

    return true;
}
