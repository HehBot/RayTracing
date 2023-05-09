#include "dielectric.h"

#include "../misc.h"
#include "../ray.h"
#include "../vec3.h"

#include <cmath>

dielectric::dielectric(double ir)
    : ir(ir)
{
}
bool dielectric::scatter(ray const& r, hit_record const& rec, color& attenuation, ray& scattered) const
{
    attenuation = color(1.0, 1.0, 1.0);
    double mu_i_by_mu_t = (rec.front_face ? (1.0 / ir) : ir);

    vec3 unit_incident = r.direction.unit_vec();
    double cos_theta = fmin(dot(-unit_incident, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    vec3 direction;
    if (mu_i_by_mu_t * sin_theta > 1.0 || reflectance(cos_theta, mu_i_by_mu_t) > random_double())
        direction = reflect(unit_incident, rec.normal);
    else
        direction = refract(unit_incident, rec.normal, mu_i_by_mu_t);

    scattered = ray(rec.p, direction, r.time);
    return true;
}
double dielectric::reflectance(double cos, double ref_ratio)
{
    double r0 = (1 - ref_ratio) / (1 + ref_ratio);
    r0 *= r0;
    return r0 + (1 - r0) * std::pow(1 - cos, 5);
}
