#ifndef QUAD_H
#define QUAD_H

#include <aabb.h>
#include <hittable.h>
#include <memory>
#include <vec3.h>

class interval;
class material;
class ray;

class quad : public hittable {
    pos3 q;
    vec3 u, v;
    std::shared_ptr<material> mat_ptr;
    aabb bbox;

    vec3 normal, w;
    double d;

public:
    quad(pos3 q, vec3 u, vec3 v, std::shared_ptr<material> m);

    bool hit(ray const& r, interval ray_t, hit_record& rec) const;
    aabb bounding_box() const;
};

#endif // QUAD_H
