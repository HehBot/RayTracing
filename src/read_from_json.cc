#include <camera.h>
#define JSON_DIAGNOSTICS 1
#include <ext/nlohmann/json.hpp>
#include <fstream>
#include <functional>
#include <hittable_list.h>
#include <initializer_list>
#include <instances/rotate.h>
#include <instances/scale.h>
#include <instances/translate.h>
#include <libgen.h>
#include <materials/dielectric.h>
#include <materials/diffuse_light.h>
#include <materials/lambertian.h>
#include <materials/metal.h>
#include <memory>
#include <misc.h>
#include <moving.h>
#include <primitives/quad.h>
#include <primitives/sphere.h>
#include <primitives/triangle.h>
#include <read_from_json.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <textures/checker_texture.h>
#include <textures/image_texture.h>
#include <textures/noise_texture.h>
#include <textures/solid_color.h>
#include <unistd.h>
#include <vec3.h>
#include <vector>

class hittable;
class material;

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

static std::shared_ptr<texture> parse_texture(json const& j)
{
    if (j["type"] == "solid_color") {
        auto c = j["color"];
        return std::make_shared<solid_color>(c[0], c[1], c[2]);
    } else if (j["type"] == "checker_texture") {
        std::shared_ptr<texture> e = parse_texture(j["even"]);
        std::shared_ptr<texture> o = parse_texture(j["odd"]);
        return std::make_shared<checker_texture>(e, o);
    } else if (j["type"] == "noise_texture")
        return std::make_shared<noise_texture>(j["scale"]);
    else if (j["type"] == "image_texture")
        return std::make_shared<image_texture>((dir + std::string("/") + std::string(j["path"])).c_str());
    else
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
    } else if (j["type"] == "dielectric")
        return std::make_shared<dielectric>(j["ir"]);
    else if (j["type"] == "diffuse_light") {
        std::shared_ptr<texture> color = parse_texture(j["color"]);
        return std::make_shared<diffuse_light>(color);
    } else
        throw std::invalid_argument("Invalid material");
}

static std::shared_ptr<hittable> parse_transformation(std::shared_ptr<hittable> obj, json const& j)
{
    if (j.size() != 1)
        throw std::invalid_argument("Invalid transform");
    if (j.contains("translate")) {
        auto offset = j["translate"];
        return std::make_shared<translate>(obj, vec3(offset[0], offset[1], offset[2]));
    } else if (j.contains("xRotation"))
        return std::make_shared<rotate_x>(obj, deg_to_rad(j["xRotation"]));
    else if (j.contains("yRotation"))
        return std::make_shared<rotate_y>(obj, deg_to_rad(j["yRotation"]));
    else if (j.contains("zRotation"))
        return std::make_shared<rotate_z>(obj, deg_to_rad(j["zRotation"]));
    else if (j.contains("scale")) {
        auto sc = j["scale"];
        return std::make_shared<scale>(obj, vec3(sc[0], sc[1], sc[2]));
    } else
        throw std::invalid_argument("Invalid transform");
}

static std::shared_ptr<hittable> parse_object(json const& j)
{
    std::shared_ptr<hittable> ret;

    if (j["type"] == "sphere") {
        auto pos = j["position"];
        std::shared_ptr<material> mat = parse_material(j["material"]);
        ret = std::make_shared<sphere>(pos3(pos[0], pos[1], pos[2]), j["radius"], mat);
    } else if (j["type"] == "quad") {
        auto pos = j["position"];
        auto u = j["u"];
        auto v = j["v"];
        std::shared_ptr<material> mat = parse_material(j["material"]);
        ret = std::make_shared<quad>(pos3(pos[0], pos[1], pos[2]), vec3(u[0], u[1], u[2]), vec3(v[0], v[1], v[2]), mat);
    } else if (j["type"] == "triangle") {
        auto v1 = j["v1"];
        auto v2 = j["v2"];
        auto v3 = j["v3"];
        std::shared_ptr<material> mat = parse_material(j["material"]);
        ret = std::make_shared<triangle>(pos3(v1[0], v1[1], v1[2]), pos3(v2[0], v2[1], v2[2]), pos3(v3[0], v3[1], v3[2]), mat);
    } else if (j["type"] == "composite") {
        std::shared_ptr<hittable_list> h = std::make_shared<hittable_list>();
        for (auto const& j_obj : j["objects"])
            h->add(parse_object(j_obj));
        ret = h;
    } else if (j["type"] == "stl_file") {
        std::shared_ptr<material> mat = parse_material(j["material"]);
        std::shared_ptr<hittable_list> h = std::make_shared<hittable_list>();
        std::ifstream file(dir + std::string("/") + std::string(j["file_path"]), std::ios::binary);
        file.seekg(80);
        uint32_t nr_triangles;
        file.read((char*)&nr_triangles, sizeof(nr_triangles));
        for (std::size_t i = 0; i < nr_triangles; ++i) {
            vec3 z[4];
            for (int i = 0; i < 4; ++i) {
                float f[3];
                file.read((char*)&f[0], sizeof(f));
                z[i] = vec3(f[0], f[1], f[2]);
            }
            // skip over attributes
            file.seekg(2, std::ios::cur);
            h->add(std::make_shared<triangle>(z[1], z[2], z[3], mat));
        }
        ret = h;
    } else
        throw std::invalid_argument("Invalid object");

    if (j.contains("transformation"))
        for (json const& j_transform : j["transformation"])
            ret = parse_transformation(ret, j_transform);

    return ret;
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

std::shared_ptr<camera> read_from_json(char const* filename, hittable_list& world)
{
    dir = get_json_dir(filename);
    json scene_spec = json::parse(std::ifstream(filename));

    json const& world_spec = scene_spec["world"];
    std::shared_ptr<texture> background = parse_texture(world_spec["background"]);

    for (auto const& z : world_spec["objects"]) {
        std::shared_ptr<hittable> obj = parse_object(z);
        if (z.contains("path"))
            obj = std::make_shared<moving>(obj, parse_path(z["path"]));
        world.add(obj);
    }

    json const& camera_spec = scene_spec["camera"];
    pos3 look_from, look_at;
    vec3 vup;
    {
        auto lf = camera_spec["look_from"], la = camera_spec["look_at"], vu = camera_spec["vup"];
        look_from = pos3(lf[0], lf[1], lf[2]);
        look_at = pos3(la[0], la[1], la[2]);
        vup = vec3(vu[0], vu[1], vu[2]);
    }

    json const& image_spec = scene_spec["image"];
    int pixel_width, pixel_height;
    if (!image_spec.contains("pixel_height")) {
        double aspect_ratio = image_spec["aspect_ratio"];
        pixel_width = image_spec["pixel_width"];
        pixel_height = pixel_width / aspect_ratio;
    } else if (!image_spec.contains("pixel_width")) {
        double aspect_ratio = image_spec["aspect_ratio"];
        pixel_height = image_spec["pixel_height"];
        pixel_width = pixel_height * aspect_ratio;
    } else {
        pixel_height = image_spec["pixel_height"];
        pixel_width = image_spec["pixel_width"];
    }
    int samples_per_pixel = image_spec["samples_per_pixel"];
    int max_depth = image_spec["max_depth"];

    return std::make_shared<camera>(pixel_width, pixel_height, samples_per_pixel, max_depth, background, look_from, look_at, vup, camera_spec["fov"], camera_spec["aperture"], camera_spec["focal_distance"]);
}
