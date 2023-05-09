#include "read_from_json.h"

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "misc.h"
#include "moving.h"
#include "primitives/sphere.h"
#include "texture.h"
#include "textures/checker_texture.h"
#include "textures/image_texture.h"
#include "textures/noise_texture.h"
#include "textures/solid_color.h"

#include <fstream>
#include <functional>
#include <libgen.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unistd.h>

using json = nlohmann::json;

static std::string dir;

std::string get_json_dir(char const* json_filename)
{
    char* z = new char[strlen(json_filename) + 1];
    strcpy(z, json_filename);
    char* f = get_current_dir_name();
    std::string dir = std::string(f) + std::string("/") + std::string(dirname(z));
    free(f);
    delete[] z;
    return dir;
}

static void parse_metadata(json const& j, metadata& m)
{
    if (!j.contains("height")) {
        m.aspect_ratio = j["aspect_ratio"];
        m.width = j["width"];
        m.height = m.width / m.aspect_ratio;
    } else if (!j.contains("width")) {
        m.aspect_ratio = j["aspect_ratio"];
        m.height = j["height"];
        m.width = m.height * m.aspect_ratio;
    } else {
        m.height = j["height"];
        m.width = j["width"];
        m.aspect_ratio = m.width / 1.0 * m.height;
    }
    m.samples_per_pixel = j["samples_per_pixel"];
    m.max_depth = j["max_depth"];
}

static std::shared_ptr<texture> parse_texture(json const& j)
{
    if (j["type"] == "solid_color") {
        auto c = j["color"];
        return std::make_shared<solid_color>(c[0], c[1], c[2]);
    } else if (j["type"] == "checker_texture") {
        std::shared_ptr<texture> e = parse_texture(j["even"]);
        std::shared_ptr<texture> o = parse_texture(j["odd"]);
        return std::make_shared<checker_texture>(e, o);
    } else if (j["type"] == "noise_texture") {
        return std::make_shared<noise_texture>(j["scale"]);
    } else if (j["type"] == "image_texture") {
        return std::make_shared<image_texture>((dir + std::string("/") + std::string(j["path"])).c_str());
    } else
        throw std::invalid_argument("Invalid texture");
}

static std::shared_ptr<material> parse_material(json const& j)
{
    if (j["type"] == "lambertian") {
        std::shared_ptr<texture> albedo = parse_texture(j["albedo"]);
        return std::make_shared<lambertian>(albedo);
    } else if (j["type"] == "metal") {
        std::shared_ptr<texture> albedo = parse_texture(j["albedo"]);
        return std::make_shared<metal>(albedo, j["fuzz"]);
    } else if (j["type"] == "dielectric") {
        return std::make_shared<dielectric>(j["ir"]);
    } else
        throw std::invalid_argument("Invalid material");
}

static std::shared_ptr<hittable> parse_object(json const& j)
{
    std::shared_ptr<material> mat = parse_material(j["material"]);

    auto pos = j["position"];
    if (j["type"] == "sphere")
        return std::make_shared<sphere>(pos3(pos[0], pos[1], pos[2]), j["radius"], mat);
    else
        throw std::invalid_argument("Invalid object");
}

static std::function<pos3(double)> parse_path(json const& j)
{
    if (j["type"] == "lerp") {
        auto p0 = j["p0"];
        auto p1 = j["p1"];
        pos3 initial(p0[0], p0[1], p0[2]);
        pos3 final(p1[0], p1[1], p1[2]);
        double time0 = j["t0"];
        double time1 = j["t1"];
        auto path = [=](double time) {
            return (initial * (time1 - time) + final * (time - time0)) / (time1 - time0);
        };
        return path;
    } else
        throw std::invalid_argument("Invalid path");
}

std::shared_ptr<camera> read_from_json(char const* filename, metadata& m, hittable_list& world)
{
    dir = get_json_dir(filename);
    json scene_spec = json::parse(std::ifstream(filename));

    parse_metadata(scene_spec["image"], m);

    {
        json const& world_spec = scene_spec["world"];
        m.background = parse_texture(world_spec["background"]);

        for (auto const& z : world_spec["objects"]) {
            std::shared_ptr<hittable> obj = parse_object(z);
            if (z.contains("path"))
                obj = std::make_shared<moving>(obj, parse_path(z["path"]));
            world.add(obj);
        }
    }

    json& camera_spec = scene_spec["camera"];
    auto look_from = camera_spec["look_from"], look_at = camera_spec["look_at"], vup = camera_spec["vup"];
    return std::make_shared<camera>(pos3(look_from[0], look_from[1], look_from[2]), pos3(look_at[0], look_at[1], look_at[2]), vec3(vup[0], vup[1], vup[2]), camera_spec["fov"], m.aspect_ratio, camera_spec["aperture"], camera_spec["focal_distance"], 0, 1);
}
