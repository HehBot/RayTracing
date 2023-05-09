#include "noise_texture.h"

#include "../misc.h"
#include "../perlin.h"
#include "../vec3.h"

#include <memory>

noise_texture::noise_texture(double sc)
    : sc(sc)
{
}
color noise_texture::value(double u, double v, vec3 const& p) const
{
    return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(sc * p.z + 10.0 * noise.turb(p)));
}
