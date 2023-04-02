#ifndef CAMERA_H
#define CAMERA_H

#include "misc.h"
#include "ray.h"

class camera {
    double const viewport_height, aspect_ratio, viewport_width;
    double const focal_length;

    pos3 const focus;
    vec3 const horizontal;
    vec3 const vertical;
    pos3 const lower_left;

public:
    camera(double v_h, double a_r, double f_l)
        : viewport_height(v_h), aspect_ratio(a_r), viewport_width(a_r * v_h), focal_length(f_l),
          focus(0.0, 0.0, 0.0),
          horizontal(viewport_width, 0.0, 0.0),
          vertical(0.0, v_h, 0.0),
          lower_left(focus - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, f_l))
    {
    }

    ray get_ray(double u, double v) const
    {
        return ray(focus, lower_left + u * horizontal + v * vertical - focus);
    }
};

#endif /* CAMERA_H */
