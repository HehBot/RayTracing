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

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& h) const override
    {
        bool any_hit = false;
        h.t = t_max;

        for (auto& p : list) {
            hit_record h_temp;
            if (p->hit(r, t_min, h.t, h_temp)) {
                any_hit = true;
                h = h_temp;
            }
        }

        return any_hit;
    }
};

#endif /* HITTABLE_LIST_H */
