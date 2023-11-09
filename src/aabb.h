#ifndef AABB_H
#define AABB_H

#include <cmath>
#include <interval.h>
#include <vec3.h>

class ray;

double constexpr min_thickness = 1e-4;

class aabb {
public:
    interval x, y, z;
    aabb() = default;
    aabb(pos3 const& a, pos3 const& b)
        : x(std::fmin(a[0], b[0]), std::fmax(a[0], b[0])),
          y(std::fmin(a[1], b[1]), std::fmax(a[1], b[1])),
          z(std::fmin(a[2], b[2]), std::fmax(a[2], b[2]))
    {
    }
    aabb(interval const& ix, interval const& iy, interval const& iz)
        : x(ix), y(iy), z(iz)
    {
    }
    aabb(aabb const& box0, aabb const& box1)
        : x(box0.x, box1.x),
          y(box0.y, box1.y),
          z(box0.z, box1.z)
    {
    }
    aabb offset(vec3 off) const
    {
        return aabb(x.offset(off[0]), y.offset(off[1]), z.offset(off[2]));
    }
    aabb scale(vec3 sc) const
    {
        return aabb(x.scale(sc[0]), y.scale(sc[1]), z.scale(sc[2]));
    }
    aabb pad() const
    {
        interval new_x = (x.size() > min_thickness ? x : x.expand(min_thickness));
        interval new_y = (y.size() > min_thickness ? y : y.expand(min_thickness));
        interval new_z = (z.size() > min_thickness ? z : z.expand(min_thickness));
        return aabb(new_x, new_y, new_z);
    }

    interval const& axis(int n) const
    {
        if (n == 1)
            return y;
        else if (n == 2)
            return z;
        return x;
    }

    inline double surface_area() const
    {
        return 2 * ((x.size() + z.size()) * y.size() + z.size() * x.size());
    }

    bool hit(ray const& r, interval ray_t) const;
};

#endif // AABB_H
