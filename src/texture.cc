#include "texture.h"

#include "misc.h"
#include "vec3.h"

#include <memory>

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
