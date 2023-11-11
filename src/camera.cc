#include <algorithm>
#include <camera.h>
#include <chrono>
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
#include <vec3.h>

camera::camera(pos3 lookfrom, pos3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist)
    : focus(lookfrom), lens_radius(aperture / 2.0)
{
    double theta = deg_to_rad(vfov);
    double viewport_height = 2 * std::tan(theta / 2);
    double viewport_width = aspect * viewport_height;

    w = (lookfrom - lookat).unit_vec();
    u = cross(vup, w);
    if (u.near_zero() && (u = cross(vec3(1.0, 0.0, 0.0), w)).near_zero() && (u = cross(vec3(0.0, 1.0, 0.0), w)).near_zero())
        u = cross(vec3(0.0, 0.0, 1.0), w);
    u = u.unit_vec();
    v = cross(w, u).unit_vec();

    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = focus - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;
}
ray camera::get_ray(double s, double t) const
{
    vec3 rd = lens_radius * random_vec_in_disc();
    vec3 offset = u * rd.x + v * rd.y;
    return ray(focus + offset, lower_left + s * horizontal + t * vertical - focus - offset, random_double(0.0, 1.0));
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
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, world, background, depth - 1);
}

std::vector<color> camera::render(metadata const& m, hittable const& world)
{
    std::vector<color> image(m.width * m.height);

    std::vector<std::thread> threads;
    int const N = std::thread::hardware_concurrency();

    auto job = [m, N, this, &image, &world](int const tid) {
        for (int c = tid; c < m.height * m.width; c += N) {
            int i = c % m.width;
            int j = c / m.width;
            color accumulator(0.0, 0.0, 0.0);
            for (int k = 0; k < m.samples_per_pixel; ++k) {
                double u = (i + random_double()) / m.width;
                double v = (j + random_double()) / m.height;
                accumulator += ray_color(get_ray(u, v), world, m.background, m.max_depth);
            }
            image[c] = accumulator / m.samples_per_pixel;
        }
    };

    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i)
        threads.emplace_back(job, i);
    for (auto& th : threads)
        th.join();

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cerr << "Time taken for render = " << std::chrono::duration<double, std::milli>(t2 - t1).count() / 1000.0 << " s using " << N << " threads\n";

    return image;
}
