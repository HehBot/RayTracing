#include "read_from_json.h"

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

camera* read_from_json(char const* filename, metadata& m, hittable_list& world)
{
    json scene_spec = json::parse(std::ifstream(filename));

    {
        json& image_spec = scene_spec["image"];
        if (!image_spec.contains("height")) {
            m.aspect_ratio = image_spec["aspect_ratio"];
            m.width = image_spec["width"];
            m.height = m.width / m.aspect_ratio;
        } else if (!image_spec.contains("width")) {
            m.aspect_ratio = image_spec["aspect_ratio"];
            m.height = image_spec["height"];
            m.width = m.height * m.aspect_ratio;
        } else {
            m.height = image_spec["height"];
            m.width = image_spec["width"];
            m.aspect_ratio = m.width / 1.0 * m.height;
        }
        m.samples_per_pixel = image_spec["samples_per_pixel"];
        m.max_depth = image_spec["max_depth"];
    }
    {
        json& world_spec = scene_spec["world"];
        auto bc = world_spec["background_color"];
        m.background_color = color(bc[0], bc[1], bc[2]);

        for (auto z : world_spec["objects"]) {
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
    }

    json& camera_spec = scene_spec["camera"];
    auto look_from = camera_spec["look_from"], look_at = camera_spec["look_at"], vup = camera_spec["vup"];
    camera* cam_ptr = new camera(pos3(look_from[0], look_from[1], look_from[2]), pos3(look_at[0], look_at[1], look_at[2]), vec3(vup[0], vup[1], vup[2]), camera_spec["fov"], m.aspect_ratio, camera_spec["aperture"], camera_spec["focal_distance"]);
    return cam_ptr;
}