#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "img_file.h"
#include "material.h"
#include "misc.h"
#include "ray.h"
#include "read_from_json.h"
#include "vec3.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

static metadata m;

color ray_color(ray const& r, hittable const& world, int depth)
{
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0.0, 0.0, 0.0);
    }
    return m.background_color;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <scene_json> <output_file>\n";
        return 0;
    }

    hittable_list world;
    camera* cam = read_from_json(argv[1], m, world);

    bvh_node W(world, cam->time0, cam->time1);

    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<color> image(m.width * m.height, { 0.0, 0.0, 0.0 });

    std::vector<std::thread> threads;
    int const N = std::thread::hardware_concurrency();

    auto job = [&](int const tid) {
        for (int c = tid; c < m.height * m.width; c += N) {
            int i = c % m.width;
            int j = c / m.width;
            color accumulator(0.0, 0.0, 0.0);
            for (int k = 0; k < m.samples_per_pixel; ++k) {
                double u = (i + random_double()) / m.width;
                double v = (j + random_double()) / m.height;
                accumulator += ray_color(cam->get_ray(u, v), W, m.max_depth);
            }
            image[c] = accumulator / m.samples_per_pixel;
        }
    };

    for (int i = 0; i < N; ++i)
        threads.emplace_back(job, i);
    for (auto& th : threads)
        th.join();

    write_to_img_file(argv[2], img_format::PPM, m.width, m.height, image);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cerr << "Done, time taken = " << std::chrono::duration<double, std::milli>(t2 - t1).count() / 1000.0 << " s using " << N << " threads\n";
    return 0;
}
