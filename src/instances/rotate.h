#ifndef ROTATE_H
#define ROTATE_H

#include "../aabb.h"
#include "../hittable.h"
#include "../ray.h"

#include <memory>

#define ROTATE(X)                                                                                   \
    class rotate_##X : public hittable {                                                            \
        std::shared_ptr<hittable> ptr;                                                              \
        double const sin_theta, cos_theta;                                                          \
                                                                                                    \
    public:                                                                                         \
        rotate_##X(std::shared_ptr<hittable> ptr, double angle);                                    \
                                                                                                    \
        virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override; \
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;     \
    }

ROTATE(x);
ROTATE(y);
ROTATE(z);

#undef ROTATE

#endif // ROTATE_H
