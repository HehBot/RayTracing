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
    virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box() const = 0;
};

#endif // HITTABLE_H
