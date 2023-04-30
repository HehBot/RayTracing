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

    sphere(pos3 const& centre, double radius, std::shared_ptr<material> m)
        : centre(centre), radius(radius), mat_ptr(m)
    {
    }

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override
    {
        vec3 ac = r.origin - centre;
        double A = r.direction.length_sq();
        double B_half = dot(r.direction, ac);
        double C = ac.length_sq() - radius * radius;
        double D_quarter = B_half * B_half - A * C;

        if (D_quarter < 0.0)
            return false;

        D_quarter = std::sqrt(D_quarter);

        double root = (-B_half - D_quarter) / A;

        if (root > t_max || root < t_min) {
            root = (-B_half + D_quarter) / A;
            if (root > t_max || root < t_min)
                return false;
        }

        rec.t = root;
        rec.p = r.at(root);
        vec3 outward_normal = (rec.p - centre) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }
};

#endif /* SPHERE_H */
