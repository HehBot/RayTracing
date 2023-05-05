#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    vec3 origin, direction;
    double time;

    ray() = default;
    ray(vec3 const& o, vec3 const& d, double tm)
        : origin(o), direction(d), time(tm)
    {
    }

    pos3 at(double t) const
    {
        return origin + t * direction;
    }
};

#endif // RAY_H
