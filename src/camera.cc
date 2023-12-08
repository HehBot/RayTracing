#include <algorithm>
#include <camera.h>
#include <cmath>
#include <constants.h>
#include <hittable.h>
#include <interval.h>
#include <iostream>
#include <material.h>
#include <memory>
#include <misc.h>
#include <ratio>
#include <ray.h>
#include <read_from_json.h>
#include <texture.h>
#include <thread>
#include <util/progress_bar.h>
#include <vec3.h>

camera::camera(int pixel_width, int pixel_height, int samples_per_pixel, int max_depth, std::shared_ptr<texture> background, pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aperture, double focus_dist)
    : pixel_width(pixel_width), pixel_height(pixel_height), samples_per_pixel(samples_per_pixel), max_depth(max_depth), background(background), focus(lookfrom), lens_radius(aperture / 2.0)
{
    double theta = deg_to_rad(vfov);
    double viewport_height = 2 * std::tan(theta / 2);
    double viewport_width = (pixel_width * viewport_height) / pixel_height;

    w = (lookfrom - lookat).unit_vec();
    u = cross(vup, w);
    if (u.near_zero() && (u = cross(vec3(1.0, 0.0, 0.0), w)).near_zero() && (u = cross(vec3(0.0, 1.0, 0.0), w)).near_zero())
        u = cross(vec3(0.0, 0.0, 1.0), w);
    u = u.unit_vec();
    v = cross(w, u).unit_vec();

    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    pos3 viewport_lower_left = focus - (horizontal + vertical) * 0.5 - focus_dist * w;

    pixel_delta_u = horizontal / pixel_width;
    pixel_delta_v = vertical / pixel_height;
    pixel_00_centre = viewport_lower_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    sqrt_spp = std::sqrt(samples_per_pixel);
    inv_sqrt_spp = 1.0 / sqrt_spp;
}
ray camera::get_ray(int i, int j, int si, int sj) const
{
    vec3 rd = lens_radius * random_vec_in_disc();
    vec3 offset = i * pixel_delta_u * rd.x + j * pixel_delta_v * rd.y;

    pos3 pixel_centre = pixel_00_centre + (i * pixel_delta_u + j * pixel_delta_v);

    auto px = -0.5 + (si + random_double()) * inv_sqrt_spp;
    auto py = -0.5 + (sj + random_double()) * inv_sqrt_spp;
    vec3 strat_offset = (px * pixel_delta_u + py * pixel_delta_v);

    pos3 pixel_sample = pixel_centre + strat_offset;

    pos3 ray_origin = focus + offset;
    pos3 ray_direction = pixel_sample - ray_origin;
    double ray_time = random_double();

    return ray(ray_origin, ray_direction, ray_time);
}

static color ray_color(ray const& r, hittable const& world, std::shared_ptr<texture> background, int depth)
{
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    hit_record rec;
    if (!world.hit(r, interval(0.001, infinity), rec)) {
        double u, v;
        get_sphere_uv(r.direction.unit_vec(), u, v);
        return background->value(u, v, r.direction);
    }

    ray scattered;
    color attenuation;
    color color_from_emission = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return color_from_emission;

    color color_from_scatter = attenuation * ray_color(scattered, world, background, depth - 1);

    return color_from_emission + color_from_scatter;
}

std::vector<color> camera::render(hittable const& world) const
{
    std::vector<color> image(pixel_width * pixel_height);

    std::vector<std::thread> threads;
    int const N = std::thread::hardware_concurrency();

    progress_bar p_bar(50, pixel_width * pixel_height, 1.0);

    auto job = [N, this, &image, &world, &p_bar](int const tid) {
        for (int c = tid; c < pixel_height * pixel_width; c += N) {
            int i = c % pixel_width;
            int j = c / pixel_width;
            color accumulator(0.0, 0.0, 0.0);
            for (int si = 0; si < sqrt_spp; ++si)
                for (int sj = 0; sj < sqrt_spp; ++sj)
                    accumulator += ray_color(get_ray(i, j, si, sj), world, background, max_depth);
            image[c] = accumulator / samples_per_pixel;
            p_bar.update();
        }
    };

    for (int i = 0; i < N; ++i)
        threads.emplace_back(job, i);
    for (auto& th : threads)
        th.join();

    return image;
}
