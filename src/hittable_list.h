#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <climits>
#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> list;

    void clear()
    {
        list.clear();
    }
    void add(std::shared_ptr<hittable> o)
    {
        list.push_back(o);
    }

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override
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
};

#endif /* HITTABLE_LIST_H */
