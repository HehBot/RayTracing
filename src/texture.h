#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "vec3.h"

#include <memory>

class texture {
public:
    virtual color value(double u, double v, pos3 const& p) const = 0;
};

class solid_color : public texture {
    color const c;

public:
    solid_color(color const& c);
    solid_color(double r, double g, double b);

    virtual color value(double u, double v, vec3 const& p) const override;
};

class checker_texture : public texture {
    std::shared_ptr<texture> even, odd;

public:
    checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd);

    virtual color value(double u, double v, vec3 const& p) const override;
};

class noise_texture : public texture {
    perlin noise;
    double sc;

public:
    noise_texture(double sc);
    virtual color value(double u, double v, vec3 const& p) const override;
};

#endif // TEXTURE_H
