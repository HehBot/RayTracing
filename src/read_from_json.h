#ifndef READ_FROM_JSON_H
#define READ_FROM_JSON_H

#include <memory>

class camera;
class hittable_list;
class texture;

struct metadata {
    int width, height;
    double aspect_ratio;
    int samples_per_pixel;
    int max_depth;
    std::shared_ptr<texture> background;
};

std::shared_ptr<camera> read_from_json(char const* filename, metadata& m, hittable_list& world);

#endif // READ_FROM_JSON_H
