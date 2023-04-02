#include "camera.h"
#include "hittable_list.h"
#include "img_file.h"
#include "misc.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

#include <cmath>
#include <iostream>
#include <vector>

color ray_color(ray const& r, hittable const& world)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
        return 0.5 * rec.normal + vec3(0.5, 0.5, 0.5);
    double t = 0.5 * r.direction.y / r.direction.length() + 0.5;
    color c = color(1.0, 1.0, 1.0) * (1.0 - t) + color(0.5, 0.7, 1.0) * t;
    return c;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file>\n";
        return 0;
    }

    // image
    double aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    int img_height = (int)(img_width / aspect_ratio);
    int samples_per_pixel = 100;

    // world
    hittable_list world;
    world.add(std::make_shared<sphere>(pos3(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<sphere>(pos3(0.0, -100.5, -1.0), 100.0));

    camera cam(2.0, aspect_ratio, 1.0);

    std::vector<color> image(img_width * img_height, { 0.0, 0.0, 0.0 });
    for (int j = 0; j < img_height; ++j) {
        for (int i = 0; i < img_width; ++i) {
            color accumulator(0.0, 0.0, 0.0);
            for (int k = 0; k < samples_per_pixel; ++k) {
                double u = (i + random_double()) / img_width;
                double v = (j + random_double()) / img_height;
                accumulator += ray_color(cam.get_ray(u, v), world);
            }
            image[img_width * j + i] = accumulator / samples_per_pixel;
        }
    }

    write_to_img_file(argv[1], img_format::PPM, img_width, img_height, image);
    return 0;
}
