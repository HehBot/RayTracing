#include "solid_color.h"

#include "../texture.h"
#include "../vec3.h"

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
