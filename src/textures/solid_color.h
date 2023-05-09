#ifndef TEXTURES_SOLID_COLOR_H
#define TEXTURES_SOLID_COLOR_H

#include "../texture.h"
#include "../vec3.h"

class solid_color : public texture {
    color const c;

public:
    solid_color(color const& c);
    solid_color(double r, double g, double b);

    virtual color value(double u, double v, vec3 const& p) const override;
};

#endif // TEXTURES_SOLID_COLOR_H
