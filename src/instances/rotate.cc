#include "rotate.h"

#include "../aabb.h"
#include "../constants.h"
#include "../hittable.h"
#include "../ray.h"

#include <cmath>
#include <memory>

#define ROTATE(X, Y, Z)                                                                   \
    rotate_##X::rotate_##X(std::shared_ptr<hittable> ptr, double angle)                   \
        : ptr(ptr), sin_theta(std::sin(angle)), cos_theta(std::cos(angle))                \
    {                                                                                     \
    }                                                                                     \
    bool rotate_##X::hit(ray const& r, double t_min, double t_max, hit_record& rec) const \
    {                                                                                     \
        vec3 origin, direction;                                                           \
                                                                                          \
        origin.X = r.origin.X;                                                            \
        origin.Y = cos_theta * r.origin.Y + sin_theta * r.origin.Z;                       \
        origin.Z = -sin_theta * r.origin.Y + cos_theta * r.origin.Z;                      \
                                                                                          \
        direction.X = r.direction.X;                                                      \
        direction.Y = cos_theta * r.direction.Y + sin_theta * r.direction.Z;              \
        direction.Z = -sin_theta * r.direction.Y + cos_theta * r.direction.Z;             \
                                                                                          \
        ray rotated_r(origin, direction, r.time);                                         \
        if (!ptr->hit(rotated_r, t_min, t_max, rec))                                      \
            return false;                                                                 \
                                                                                          \
        vec3 p, normal;                                                                   \
                                                                                          \
        p.X = rec.p.X;                                                                    \
        p.Y = cos_theta * rec.p.Y - sin_theta * rec.p.Z;                                  \
        p.Z = sin_theta * rec.p.Y + cos_theta * rec.p.Z;                                  \
        normal.X = rec.normal.X;                                                          \
        normal.Y = cos_theta * rec.normal.Y - sin_theta * rec.normal.Z;                   \
        normal.Z = sin_theta * rec.normal.Y + cos_theta * rec.normal.Z;                   \
                                                                                          \
        rec.p = p;                                                                        \
        rec.set_face_normal(r, normal);                                                   \
        return true;                                                                      \
    }                                                                                     \
    bool rotate_##X::bounding_box(double time0, double time1, aabb& output_box) const     \
    {                                                                                     \
        aabb bbox;                                                                        \
        bool hasbox = ptr->bounding_box(time0, time1, bbox);                              \
                                                                                          \
        if (!hasbox)                                                                      \
            return false;                                                                 \
                                                                                          \
        pos3 min(infinity, infinity, infinity);                                           \
        pos3 max(-infinity, -infinity, -infinity);                                        \
                                                                                          \
        for (int i = 0; i < 2; ++i) {                                                     \
            for (int j = 0; j < 2; ++j) {                                                 \
                for (int k = 0; k < 2; ++k) {                                             \
                    auto X = i * bbox.max.X + (1 - i) * bbox.min.X;                       \
                    auto Y = j * bbox.max.Y + (1 - j) * bbox.min.Y;                       \
                    auto Z = k * bbox.max.Z + (1 - k) * bbox.min.Z;                       \
                                                                                          \
                    auto new##Y = cos_theta* Y - sin_theta* Z;                            \
                    auto new##Z = sin_theta* Y + cos_theta* Z;                            \
                                                                                          \
                    min.X = fmin(min.X, X);                                               \
                    min.Y = fmin(min.Y, new##Y);                                          \
                    min.Z = fmin(min.Z, new##Z);                                          \
                    max.X = fmax(max.X, X);                                               \
                    max.Y = fmax(max.Y, new##Y);                                          \
                    max.Z = fmax(max.Z, new##Z);                                          \
                }                                                                         \
            }                                                                             \
        }                                                                                 \
        output_box = aabb(min, max);                                                      \
        return true;                                                                      \
    }

ROTATE(x, y, z)
ROTATE(y, z, x)
ROTATE(z, x, y)
