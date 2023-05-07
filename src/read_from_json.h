#ifndef READ_FROM_JSON_H
#define READ_FROM_JSON_H

#include "camera.h"
#include "hittable_list.h"

#include <memory>

struct metadata {
    int width, height;
    double aspect_ratio;
    int samples_per_pixel;
    int max_depth;
    color background_color;
};

std::shared_ptr<camera> read_from_json(char const* filename, metadata& m, hittable_list& world);

#endif // READ_FROM_JSON_H
