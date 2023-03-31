#ifndef VEC3_H
#define VEC3_H

#include <cmath>

class vec3 {
public:
    double x, y, z;

    vec3() = default;
    vec3(double x, double y, double z)
        : x(x), y(y), z(z)
    {
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
    vec3& operator/=(double t)
    {
        return ((*this) *= (1.0 / t));
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

using color = vec3;
using pos3 = vec3;

#endif // VEC3_H
