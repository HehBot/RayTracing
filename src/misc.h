#ifndef MISC_H
#define MISC_H

#include "constants.h"
#include "vec3.h"

#include <cmath>

inline double deg_to_rad(double deg)
{
    return deg * pi / 180.0;
}

double random_double();
inline double random_double(double a, double b)
{
    return a + (b - a) * random_double();
}
// random integer in [a, b)
inline int random_int(int a, int b)
{
    return (int)(random_double(a, b));
}
inline vec3 random_unit_vec()
{
    double theta = 2 * pi * random_double();
    double z = 2 * random_double() - 1.0;
    double r = std::sqrt(1.0 - z * z);
    return vec3(r * std::cos(theta), r * std::sin(theta), z);
}
inline vec3 random_vec_in_sphere()
{
    double r = std::pow(random_double(), 0.33333);
    return r * random_unit_vec();
}
inline vec3 random_vec_in_disc()
{
    double r = std::pow(random_double(), 0.5);
    double theta = 2 * pi * random_double();
    return vec3(r * std::cos(theta), r * std::sin(theta), 0);
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

void get_sphere_uv(pos3 const& p, double& u, double& v);

#endif // MISC_H
