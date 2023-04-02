#ifndef MISC_H
#define MISC_H

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

#endif /* MISC_H */
