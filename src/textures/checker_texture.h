#ifndef TEXTURES_CHECKER_TEXTURE_H
#define TEXTURES_CHECKER_TEXTURE_H

#include "../texture.h"
#include "../vec3.h"

#include <memory>

class checker_texture : public texture {
    std::shared_ptr<texture> even, odd;

public:
    checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd);

    virtual color value(double u, double v, vec3 const& p) const override;
};

#endif // TEXTURES_CHECKER_TEXTURE_H
