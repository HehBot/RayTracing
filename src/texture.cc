#include "texture.h"

#include "misc.h"
#include "vec3.h"

#include <iostream>
#include <memory>
#include <stb/stb_image.h>

solid_color::solid_color(color const& c)
    : c(c)
{
}
solid_color::solid_color(double r, double g, double b)
    : c(r, g, b)
{
}
color solid_color::value(double u, double v, vec3 const& p) const
{
    return c;
}

checker_texture::checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
    : even(even), odd(odd)
{
}
color checker_texture::value(double u, double v, vec3 const& p) const
{
    double z = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (z > 0)
        return even->value(u, v, p);
    return odd->value(u, v, p);
}

noise_texture::noise_texture(double sc)
    : sc(sc)
{
}
color noise_texture::value(double u, double v, vec3 const& p) const
{
    return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(sc * p.z + 10.0 * noise.turb(p)));
}

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
