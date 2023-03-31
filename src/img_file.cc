#include "img_file.h"

#include "vec3.h"

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <vector>

void write_to_img_file(char const* filename, img_format f, int w, int h, std::vector<color> const& image)
{
    switch (f) {
    case img_format::PPM:
        if (w < 0 || h < 0 || (int)image.size() != w * h)
            throw std::invalid_argument("Invalid image dimensions");
        std::vector<uint8_t> pixels(3 * w * h, 0);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                pixels[3 * (w * j + i)] = (uint8_t)(255.999 * image[w * (h - 1 - j) + i].x);
                pixels[3 * (w * j + i) + 1] = (uint8_t)(255.999 * image[w * (h - 1 - j) + i].y);
                pixels[3 * (w * j + i) + 2] = (uint8_t)(255.999 * image[w * (h - 1 - j) + i].z);
            }
        }
        std::ofstream output_file(filename, std::ios::binary);
        std::string header = "P6 " + std::to_string(w) + " " + std::to_string(h) + " 255 ";
        output_file.write(header.c_str(), header.length());
        output_file.write((char*)&pixels[0], 3 * w * h);
        break;
    }
}
