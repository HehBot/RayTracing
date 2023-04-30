#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class sphere : public hittable {
public:
    pos3 centre;
    double radius;
    std::shared_ptr<material> mat_ptr;

    sphere(pos3 const& centre, double radius, std::shared_ptr<material> m);

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
};

#endif /* SPHERE_H */
