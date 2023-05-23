#ifndef BOX_H
#define BOX_H

#include "../hittable_list.h"
#include "../material.h"
#include "../vec3.h"

#include <memory>

class box : public hittable {
    pos3 const p0, p1;
    hittable_list sides;

public:
    box() = default;
    box(pos3 const& p0, pos3 const& p1, std::shared_ptr<material> mat_ptr);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif // BOX_H
