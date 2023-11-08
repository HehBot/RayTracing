#ifndef TEXTURES_NOISE_TEXTURE_H
#define TEXTURES_NOISE_TEXTURE_H

#include <perlin.h>
#include <texture.h>
#include <vec3.h>

class noise_texture : public texture {
    perlin noise;
    double sc;

public:
    noise_texture(double sc);
    virtual color value(double u, double v, vec3 const& p) const override;
};

#endif // TEXTURES_NOISE_TEXTURE_H
