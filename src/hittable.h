#ifndef HITTABLE_H
#define HITTABLE_H

#include <aabb.h>
#include <interval.h>
#include <material.h>
#include <memory>
#include <ray.h>
#include <vec3.h>

class material;

struct hit_record {
    pos3 p;
    vec3 normal;
    double t;
    bool front_face;
    double u, v;
    std::shared_ptr<material> mat_ptr;

    inline void set_face_normal(ray const& r, vec3 const& outward_normal)
    {
        front_face = (dot(r.direction, outward_normal) < 0.0);
        normal = (front_face ? outward_normal : -outward_normal).unit_vec();
    }
};

class hittable {
public:
    pos3 const position;
    hittable()
        : position(0.0, 0.0, 0.0)
    {
    }
    hittable(pos3 p)
        : position(p)
    {
    }
    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box(double time0, double time1) const = 0;
};

#endif // HITTABLE_H
