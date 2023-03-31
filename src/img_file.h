#ifndef IMG_FILE_H
#define IMG_FILE_H

#include "vec3.h"

#include <vector>

enum class img_format {
    PPM,
};

void write_to_img_file(char const* filename, img_format f, int w, int h, std::vector<color> const& pixels);

#endif // IMG_FILE_H
