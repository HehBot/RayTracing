#ifndef CAMERA_H
#define CAMERA_H

#include "misc.h"
#include "ray.h"

class camera {
    pos3 const focus;
    vec3 horizontal;
    vec3 vertical;
    pos3 lower_left;

    vec3 u, v, w;
    double const lens_radius;

    double const time0, time1;

public:
    camera(pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0, double t1);
    ray get_ray(double u, double v) const;
};

#endif /* CAMERA_H */
