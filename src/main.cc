#include "camera.h"
#include "hittable_list.h"
#include "img_file.h"
#include "misc.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <vector>

using json = nlohmann::json;

static color background_color;

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
    return background_color;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <scene_json> <output_file>\n";
        return 0;
    }

    json scene_spec = json::parse(std::ifstream(argv[1]));

    // image
    json& image_spec = scene_spec["image"];
    double aspect_ratio = image_spec["aspect_ratio"];
    int img_width = image_spec["width"];
    int img_height = (int)(img_width / aspect_ratio);
    int samples_per_pixel = image_spec["samples_per_pixel"];
    int max_depth = image_spec["max_depth"];

    // world
    auto bc = scene_spec["geometry"]["background_color"];
    background_color = color(bc[0], bc[1], bc[2]);

    hittable_list world;

    for (auto z : scene_spec["geometry"]["objects"]) {
        json& material_spec = z["material"];
        std::shared_ptr<material> mat;
        std::shared_ptr<hittable> obj;
        if (material_spec["type"] == "lambertian") {
            auto al = material_spec["albedo"];
            mat = std::make_shared<lambertian>(color(al[0], al[1], al[2]));
        } else if (material_spec["type"] == "metal") {
            auto al = material_spec["albedo"];
            mat = std::make_shared<metal>(color(al[0], al[1], al[2]), material_spec["fuzz"]);
        } else if (material_spec["type"] == "dielectric") {
            mat = std::make_shared<dielectric>(material_spec["ir"]);
        } else
            throw std::invalid_argument("Invalid material");
        if (z["type"] == "sphere") {
            auto pos = z["position"];
            obj = std::make_shared<sphere>(pos3(pos[0], pos[1], pos[2]), z["radius"], mat);
        } else
            throw std::invalid_argument("Invalid object");
        world.add(obj);
    }

    // camera
    json& camera_spec = scene_spec["camera"];
    auto look_from = camera_spec["look_from"], look_at = camera_spec["look_at"], vup = camera_spec["vup"];
    camera cam(pos3(look_from[0], look_from[1], look_from[2]), pos3(look_at[0], look_at[1], look_at[2]), vec3(vup[0], vup[1], vup[2]), camera_spec["fov"], aspect_ratio, camera_spec["aperture"], camera_spec["focal_distance"]);

    std::vector<color> image(img_width * img_height, { 0.0, 0.0, 0.0 });
    for (int j = 0; j < img_height; ++j) {
        for (int i = 0; i < img_width; ++i) {
            color accumulator(0.0, 0.0, 0.0);
            for (int k = 0; k < samples_per_pixel; ++k) {
                double u = (i + random_double()) / img_width;
                double v = (j + random_double()) / img_height;
                accumulator += ray_color(cam.get_ray(u, v), world, max_depth);
            }
            image[img_width * j + i] = accumulator / samples_per_pixel;
        }
    }

    write_to_img_file(argv[2], img_format::PPM, img_width, img_height, image);
    return 0;
}
