#ifndef ROTATE_H
#define ROTATE_H

#include <hittable.h>
#include <memory>

class aabb;
class ray;

#define ROTATE(X)                                                                       \
    class rotate_##X : public hittable {                                                \
        std::shared_ptr<hittable> ptr;                                                  \
        double const sin_theta, cos_theta;                                              \
                                                                                        \
    public:                                                                             \
        rotate_##X(std::shared_ptr<hittable> ptr, double angle);                        \
                                                                                        \
        virtual bool hit(ray const& r, interval ray_t, hit_record& rec) const override; \
        virtual aabb bounding_box(double time0, double time1) const override;           \
    }

ROTATE(x);
ROTATE(y);
ROTATE(z);

#undef ROTATE

#endif // ROTATE_H
