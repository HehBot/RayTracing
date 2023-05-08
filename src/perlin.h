#ifndef PERLIN_H
#define PERLIN_H

#include "vec3.h"

class perlin {
    static int constexpr point_count = 256;
    vec3 const* ranvec;
    int const* perm_x;
    int const* perm_y;
    int const* perm_z;

public:
    perlin();
    ~perlin();

    double noise(pos3 const& p) const;
    double turb(pos3 const& p, int depth = 7) const;
};

#endif // PERLIN_H
