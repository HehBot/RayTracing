#include "perlin.h"

#include "misc.h"
#include "vec3.h"

static int* generate_perm(int n)
{
    int* p = new int[n];
    for (int i = 0; i < n; ++i) {
        int j = random_int(0, i + 1);
        if (j != i)
            p[i] = p[j];
        p[j] = i;
    }
    return p;
}

static double perlin_interp(vec3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double a = 0.0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                vec3 weight(u - i, v - j, w - k);
                a += (i * uu + (1 - i) * (1.0 - uu)) * (j * vv + (1 - j) * (1.0 - vv)) * (k * ww + (1 - k) * (1.0 - ww)) * dot(c[i][j][k], weight);
            }

    return a;
}

perlin::perlin()
    : perm_x(generate_perm(point_count)), perm_y(generate_perm(point_count)), perm_z(generate_perm(point_count))
{
    vec3* a = new vec3[point_count];
    for (std::size_t i = 0; i < point_count; ++i)
        a[i] = random_unit_vec();
    ranvec = (vec3 const*)a;
}
perlin::~perlin()
{
    delete[] ranvec;
    delete[] perm_x;
    delete[] perm_y;
    delete[] perm_z;
}
double perlin::noise(pos3 const& p) const
{
    auto u = p.x - floor(p.x);
    auto v = p.y - floor(p.y);
    auto w = p.z - floor(p.z);

    auto i = (int)floor(p.x);
    auto j = (int)floor(p.y);
    auto k = (int)floor(p.z);

    int constexpr C = point_count - 1;

    vec3 c[2][2][2];

    for (int di = 0; di < 2; ++di)
        for (int dj = 0; dj < 2; ++dj)
            for (int dk = 0; dk < 2; ++dk)
                c[di][dj][dk] = ranvec[perm_x[(i + di) & C] ^ perm_y[(j + dj) & C] ^ perm_z[(k + dk) & C]];

    return perlin_interp(c, u, v, w);
}
double perlin::turb(pos3 const& p, int depth) const
{
    double a = 0.0;
    pos3 temp_p = p;
    double w = 1.0;

    for (int i = 0; i < depth; ++i) {
        a += w * noise(temp_p);
        w *= 0.5;
        temp_p *= 2;
    }

    return fabs(a);
}
