#include "image_texture.h"

#include "../misc.h"
#include "../texture.h"
#include "../vec3.h"

#include <iostream>
#include <stb/stb_image.h>

image_texture::image_texture(char const* path)
{
    data = stbi_load(path, &w, &h, nullptr, bytes_per_pixel);

    if (!data) {
        std::cerr << "ERROR: Could not load texture image file '" << path << "'\n";
        w = h = 0;
    }

    bytes_per_scanline = bytes_per_pixel * w;
}
image_texture::~image_texture()
{
    stbi_image_free(data);
}
color image_texture::value(double u, double v, vec3 const& p) const
{
    if (data == nullptr)
        return color(0.0, 1.0, 1.0);

    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    auto i = (int)(u * w);
    auto j = (int)(v * h);

    if (i >= w)
        i = w - 1;
    if (j >= h)
        j = h - 1;

    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
    return color(pixel[0], pixel[1], pixel[2]) / 255.0;
}
