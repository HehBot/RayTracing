#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <climits>
#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    hittable_list();
    std::vector<std::shared_ptr<hittable>> list;

    void clear();
    void add(std::shared_ptr<hittable> o);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
};

#endif /* HITTABLE_LIST_H */
