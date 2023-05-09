#ifndef TEXTURES_IMAGE_TEXTURE_H
#define TEXTURES_IMAGE_TEXTURE_H

#include "../texture.h"
#include "../vec3.h"

class image_texture : public texture {
    unsigned char* data;
    int w, h, bytes_per_scanline;
    static constexpr int bytes_per_pixel = 3;

public:
    image_texture(char const* path);
    ~image_texture();
    virtual color value(double u, double v, vec3 const& p) const override;
};

#endif // TEXTURES_IMAGE_TEXTURE_H
