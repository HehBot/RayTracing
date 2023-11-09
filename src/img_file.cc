#include <cmath>
#include <cstdint>
#include <fstream>
#include <img_file.h>
#include <misc.h>
#include <stdexcept>
#include <string>
#include <vec3.h>
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
                pixels[3 * (w * j + i)] = (uint8_t)(255.999 * clamp(std::sqrt(image[w * (h - 1 - j) + i].x), 0.0, 1.0));
                pixels[3 * (w * j + i) + 1] = (uint8_t)(255.999 * clamp(std::sqrt(image[w * (h - 1 - j) + i].y), 0.0, 1.0));
                pixels[3 * (w * j + i) + 2] = (uint8_t)(255.999 * clamp(std::sqrt(image[w * (h - 1 - j) + i].z), 0.0, 1.0));
            }
        }
        std::ofstream output_file(filename, std::ios::binary);
        std::string header = "P6 " + std::to_string(w) + " " + std::to_string(h) + " 255 ";
        output_file.write(header.c_str(), header.length());
        output_file.write((char*)&pixels[0], 3 * w * h);
        break;
    }
}
