#ifndef ROTATE_H
#define ROTATE_H

#include <aabb.h>
#include <hittable.h>
#include <memory>

class interval;
class ray;

#define ROTATE(X)                                                                       \
    class rotate_##X : public hittable {                                                \
        std::shared_ptr<hittable> ptr;                                                  \
        double const sin_theta, cos_theta;                                              \
        aabb bbox;                                                                      \
                                                                                        \
    public:                                                                             \
        rotate_##X(std::shared_ptr<hittable> ptr, double angle);                        \
                                                                                        \
        virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override; \
        virtual aabb bounding_box() const override;                                     \
    }

ROTATE(x);
ROTATE(y);
ROTATE(z);

#undef ROTATE

#endif // ROTATE_H
