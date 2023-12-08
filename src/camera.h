#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <ray.h>
#include <vec3.h>
#include <vector>

class hittable;
class texture;

class camera {
public:
    int pixel_width, pixel_height;
    int samples_per_pixel;
    int max_depth;
    std::shared_ptr<texture> background;

private:
    pos3 const focus;
    vec3 horizontal;
    vec3 vertical;
    pos3 pixel_00_centre;

    vec3 u, v, w;
    double const lens_radius;

    vec3 pixel_delta_u, pixel_delta_v;

    int sqrt_spp;
    double inv_sqrt_spp;

public:
    camera(int pixel_width, int pixel_height, int samples_per_pixel, int max_depth, std::shared_ptr<texture> background, pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aperture, double focus_dist);

    std::vector<color> render(hittable const& world) const;

private:
    ray get_ray(int i, int j, int si, int sj) const;
};

#endif // CAMERA_H
