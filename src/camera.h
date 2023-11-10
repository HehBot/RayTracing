#ifndef CAMERA_H
#define CAMERA_H

#include <ray.h>
#include <vec3.h>
#include <vector>

class hittable;
struct metadata;

class camera {
    pos3 const focus;
    vec3 horizontal;
    vec3 vertical;
    pos3 lower_left;

    vec3 u, v, w;
    double const lens_radius;

    ray get_ray(double u, double v) const;

public:
    camera(pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist);

    std::vector<color> render(metadata const& m, hittable const& world);
};

#endif // CAMERA_H
