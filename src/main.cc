#include <bvh.h>
#include <camera.h>
#include <hittable_list.h>
#include <img_file.h>
#include <iostream>
#include <memory>
#include <read_from_json.h>
#include <vec3.h>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <scene_json> <output_file>\n";
        return 0;
    }

    metadata m;
    hittable_list world;
    std::shared_ptr<camera> cam = read_from_json(argv[1], m, world);

    bvh_node W(world, cam->time0, cam->time1);

    std::vector<color> image = cam->render(m, W);

    write_to_img_file(argv[2], img_format::PPM, m.width, m.height, image);

    return 0;
}
