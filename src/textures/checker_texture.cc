#include "checker_texture.h"

#include "../misc.h"
#include "../texture.h"
#include "../vec3.h"

#include <memory>

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
