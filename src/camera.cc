#include "camera.h"

#include "misc.h"
#include "ray.h"

camera::camera(pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double time0, double time1)
    : focus(lookfrom), lens_radius(aperture / 2.0), time0(time0), time1(time1)
{
    double theta = deg_to_rad(vfov);
    double viewport_height = 2 * std::tan(theta / 2);
    double viewport_width = aspect * viewport_height;

    w = (lookfrom - lookat).unit_vec();
    u = cross(vup, w).unit_vec();
    v = cross(w, u).unit_vec();

    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = focus - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;
}
ray camera::get_ray(double s, double t) const
{
    vec3 rd = lens_radius * random_vec_in_disc();
    vec3 offset = u * rd.x + v * rd.y;
    return ray(focus + offset, lower_left + s * horizontal + t * vertical - focus - offset, random_double(time0, time1));
}
