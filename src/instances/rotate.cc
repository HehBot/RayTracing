#include "rotate.h"

#include <aabb.h>
#include <cmath>
#include <constants.h>
#include <hittable.h>
#include <interval.h>
#include <memory>
#include <ray.h>
#include <vec3.h>

#define ROTATE(X, Y, Z)                                                       \
    rotate_##X::rotate_##X(std::shared_ptr<hittable> ptr, double angle)       \
        : ptr(ptr), sin_theta(std::sin(angle)), cos_theta(std::cos(angle))    \
    {                                                                         \
        bbox = ptr->bounding_box();                                           \
                                                                              \
        pos3 min(infinity, infinity, infinity);                               \
        pos3 max(-infinity, -infinity, -infinity);                            \
                                                                              \
        for (int i = 0; i < 2; ++i) {                                         \
            for (int j = 0; j < 2; ++j) {                                     \
                for (int k = 0; k < 2; ++k) {                                 \
                    auto X = i * bbox.X.max + (1 - i) * bbox.X.min;           \
                    auto Y = j * bbox.Y.max + (1 - j) * bbox.Y.min;           \
                    auto Z = k * bbox.Z.max + (1 - k) * bbox.Z.min;           \
                                                                              \
                    auto new##Y = cos_theta* Y - sin_theta* Z;                \
                    auto new##Z = sin_theta* Y + cos_theta* Z;                \
                                                                              \
                    min.X = std::fmin(min.X, X);                              \
                    min.Y = std::fmin(min.Y, new##Y);                         \
                    min.Z = std::fmin(min.Z, new##Z);                         \
                    max.X = std::fmax(max.X, X);                              \
                    max.Y = std::fmax(max.Y, new##Y);                         \
                    max.Z = std::fmax(max.Z, new##Z);                         \
                }                                                             \
            }                                                                 \
        }                                                                     \
        bbox = aabb(min, max);                                                \
    }                                                                         \
    bool rotate_##X::hit(ray const& r, interval ray_t, hit_record& rec) const \
    {                                                                         \
        vec3 origin, direction;                                               \
                                                                              \
        origin.X = r.origin.X;                                                \
        origin.Y = cos_theta * r.origin.Y + sin_theta * r.origin.Z;           \
        origin.Z = -sin_theta * r.origin.Y + cos_theta * r.origin.Z;          \
                                                                              \
        direction.X = r.direction.X;                                          \
        direction.Y = cos_theta * r.direction.Y + sin_theta * r.direction.Z;  \
        direction.Z = -sin_theta * r.direction.Y + cos_theta * r.direction.Z; \
                                                                              \
        ray rotated_r(origin, direction, r.time);                             \
        if (!ptr->hit(rotated_r, ray_t, rec))                                 \
            return false;                                                     \
                                                                              \
        vec3 p, normal;                                                       \
                                                                              \
        p.X = rec.p.X;                                                        \
        p.Y = cos_theta * rec.p.Y - sin_theta * rec.p.Z;                      \
        p.Z = sin_theta * rec.p.Y + cos_theta * rec.p.Z;                      \
        normal.X = rec.normal.X;                                              \
        normal.Y = cos_theta * rec.normal.Y - sin_theta * rec.normal.Z;       \
        normal.Z = sin_theta * rec.normal.Y + cos_theta * rec.normal.Z;       \
                                                                              \
        rec.p = p;                                                            \
        rec.set_face_normal(r, normal);                                       \
        return true;                                                          \
    }                                                                         \
    aabb rotate_##X::bounding_box() const                                     \
    {                                                                         \
        return bbox;                                                          \
    }

ROTATE(x, y, z)
ROTATE(y, z, x)
ROTATE(z, x, y)
