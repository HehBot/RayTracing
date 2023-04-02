#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class sphere : public hittable {
public:
    pos3 centre;
    double radius;

    sphere(pos3 const& centre, double radius)
        : centre(centre), radius(radius)
    {
    }

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& h) const override
    {
        vec3 ac = r.origin - centre;
        double A = r.direction.length_sq();
        double B_half = dot(r.direction, ac);
        double C = ac.length_sq() - radius * radius;
        double D_quarter = B_half * B_half - A * C;

        if (D_quarter < 0.0)
            return false;

        double root = -(B_half + std::sqrt(D_quarter)) / A;

        if (root > t_max || root < t_min) {
            root = -(B_half - std::sqrt(D_quarter)) / A;
            if (root > t_max || root < t_min)
                return false;
        }

        h.t = root;
        h.p = r.at(root);
        vec3 outward_normal = (h.p - centre) / radius;
        h.set_face_normal(r, outward_normal);

        return true;
    }
};

#endif /* SPHERE_H */
