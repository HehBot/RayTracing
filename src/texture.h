#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"

class texture {
public:
    virtual color value(double u, double v, pos3 const& p) const = 0;
};

#endif // TEXTURE_H
