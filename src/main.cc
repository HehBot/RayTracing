#include "img_file.h"
#include "ray.h"
#include "vec3.h"

#include <cmath>
#include <iostream>
#include <vector>

double hit_sphere(pos3 const& centre, double radius, ray const& r)
{
    vec3 ac = r.origin - centre;
    double A = r.direction.length_sq();
    double B_half = dot(r.direction, ac);
    double C = ac.length_sq() - radius * radius;
    double D_quarter = B_half * B_half - A * C;

    if (D_quarter < 0.0)
        return -1.0;
    return -(B_half + std::sqrt(D_quarter)) / A;
}

color ray_color(ray const& r)
{
    pos3 sphere_centre(0.0, 0.0, -1.0);
    double sphere_radius = 0.5;

    double t = hit_sphere(sphere_centre, sphere_radius, r);
    if (t < 0.0) {
        t = 0.5 * r.direction.y / r.direction.length() + 0.5;
        color c = color(1.0, 1.0, 1.0) * (1.0 - t) + color(0.5, 0.7, 1.0) * t;
        return c;
    }
    vec3 N = (r.at(t) - sphere_centre).unit_vec();
    return 0.5 * N + vec3(0.5, 0.5, 0.5);
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

    // camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    pos3 focus(0.0, 0.0, 0.0);
    vec3 viewport_hor = vec3(viewport_width, 0.0, 0.0);
    vec3 viewport_vert = vec3(0.0, viewport_height, 0.0);
    pos3 viewport_lower_left = focus - viewport_hor / 2.0 - viewport_vert / 2.0 - vec3(0.0, 0.0, focal_length);

    std::vector<color> image(img_width * img_height, { 0.0, 0.0, 0.0 });
    for (int j = 0; j < img_height; ++j) {
        for (int i = 0; i < img_width; ++i) {
            double u = ((double)i) / (img_width - 1);
            double v = ((double)j) / (img_height - 1);
            ray r(focus, viewport_lower_left + u * viewport_hor + v * viewport_vert - focus);
            image[img_width * j + i] = ray_color(r);
        }
    }

    write_to_img_file(argv[1], img_format::PPM, img_width, img_height, image);
    return 0;
}
