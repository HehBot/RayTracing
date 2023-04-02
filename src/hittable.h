#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

struct hit_record {
    pos3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(ray const& r, vec3 const& outward_normal)
    {
        front_face = dot(r.direction, outward_normal);
        normal = (front_face ? outward_normal : -outward_normal);
    }
};

class hittable {
public:
    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& h) const = 0;
};

#endif /* HITTABLE_H */
