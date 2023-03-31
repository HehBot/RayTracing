#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    vec3 origin, direction;

    ray() = default;
    ray(vec3 const& o, vec3 const& d)
        : origin(o), direction(d)
    {
    }

    pos3 at(double t) const
    {
        return origin + t * direction;
    }
};

#endif // RAY_H
