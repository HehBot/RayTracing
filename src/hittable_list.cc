#include "hittable_list.h"

#include "ray.h"

#include <memory>

hittable_list::hittable_list()
    : hittable(pos3(0.0, 0.0, 0.0))
{
}
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
