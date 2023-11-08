#include "noise_texture.h"

#include <cmath>
#include <perlin.h>
#include <vec3.h>

noise_texture::noise_texture(double sc)
    : sc(sc)
{
}
color noise_texture::value(double u, double v, vec3 const& p) const
{
    return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + std::sin(sc * p.z + 10.0 * noise.turb(p)));
}
