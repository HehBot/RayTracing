#ifndef MISC_H
#define MISC_H

#include "vec3.h"

#include <cmath>
#include <limits>
#include <random>

double constexpr infinity = std::numeric_limits<double>::infinity();
double constexpr pi = 3.1415926535897932385;

inline double deg_to_rad(double deg)
{
    return deg * pi / 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
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

#endif /* MISC_H */
