#include <cmath>
#include <constants.h>
#include <misc.h>
#include <random>
#include <vec3.h>

double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

void get_sphere_uv(pos3 const& p, double& u, double& v)
{
    double theta = std::acos(-p.y);
    double phi = std::atan2(-p.z, p.x) + pi;

    u = 0.5 * phi / pi;
    v = theta / pi;
}
