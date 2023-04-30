#include "camera.h"

#include "misc.h"
#include "ray.h"

camera::camera(pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aspect)
    : focus(lookfrom)
{
    double theta = deg_to_rad(vfov);
    double viewport_height = 2 * std::tan(theta / 2);
    double viewport_width = aspect * viewport_height;

    double focal_length = 1.0;

    vec3 looking = lookat - lookfrom;

    horizontal = viewport_width * cross(looking, vup).unit_vec();
    vertical = viewport_height * cross(horizontal, looking).unit_vec();
    lower_left = pos3(focus - horizontal / 2.0 - vertical / 2.0 + focal_length * looking.unit_vec());
}
ray camera::get_ray(double u, double v) const
{
    return ray(focus, lower_left + u * horizontal + v * vertical - focus);
}
