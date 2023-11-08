#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <constants.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

class vec3 {
public:
    double x, y, z;

    vec3() = default;
    vec3(double x, double y, double z)
        : x(x), y(y), z(z)
    {
    }

    double& operator[](std::size_t i)
    {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::invalid_argument("Bad vector component");
        }
    }
    double operator[](std::size_t i) const
    {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::invalid_argument("Bad vector component");
        }
    }

    vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }
    vec3& operator+=(vec3 const& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    vec3& operator-=(vec3 const& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    vec3& operator*=(double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    vec3& operator*=(vec3 const& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    vec3& operator/=(double t)
    {
        return ((*this) *= (1.0 / t));
    }
    vec3& operator/=(vec3 const& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    double length_sq() const
    {
        return x * x + y * y + z * z;
    }
    double length() const
    {
        return std::sqrt(length_sq());
    }

    vec3 operator+(vec3 const& v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    vec3 operator-(vec3 const& v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }
    vec3 operator*(vec3 const& v) const
    {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    vec3 operator*(double t) const
    {
        return vec3(x * t, y * t, z * t);
    }
    vec3 operator/(double t) const
    {
        return (*this) * (1 / t);
    }
    vec3 operator/(vec3 const& v) const
    {
        return vec3(x / v.x, y / v.y, z / v.z);
    }

    vec3 unit_vec() const
    {
        return (*this) / length();
    }
    bool near_zero() const
    {
        return length() < epsilon;
    }
};

inline double dot(vec3 const& v1, vec3 const& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline vec3 cross(vec3 const& v1, vec3 const& v2)
{
    return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
inline vec3 operator*(double t, vec3 const& v)
{
    return v * t;
}

inline std::ostream& operator<<(std::ostream& o, vec3 const& v)
{
    return (o << v.x << ' ' << v.y << ' ' << v.z);
}

inline vec3 reflect(vec3 const& v, vec3 const& n)
{
    return v - 2 * dot(v, n) * n;
}
inline vec3 refract(vec3 v_in, vec3 const& n, double mu_i_by_mu_t)
{
    //    v_in = v_in.unit_vec();
    double cos_theta = std::fmin(dot(-v_in, n), 1.0);
    vec3 perp = mu_i_by_mu_t * (v_in + cos_theta * n);
    vec3 parallel = -std::sqrt(1.0 - perp.length_sq()) * n;
    return parallel + perp;
}

using color = vec3;
using pos3 = vec3;

#endif // VEC3_H
