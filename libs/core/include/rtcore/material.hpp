#pragma once

#include "rtmath/vec.hpp"
#include <optional>
#include <rtcore/hittable.hpp>
#include <rtmath/ray.hpp>

namespace rtcore {

inline double schlick(double cosine, double ref_idx) {
  auto r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

struct scatter_record {
  rtmath::color attenuation;
  rtmath::ray scattered;
};

class material {
public:
  virtual ~material() {}
  virtual std::optional<scatter_record>
  scatter(const rtmath::ray &r_in, const hit_record &rec) const = 0;
};

class lambertian : public material {
public:
  lambertian(const rtmath::color &a) : albedo(a) {}

  virtual std::optional<scatter_record>
  scatter(const rtmath::ray &r_in, const hit_record &rec) const {
    rtmath::vec3 scatter_direction =
        rec.normal + rtmath::random_unit_vector<double, 3>();
    return scatter_record{albedo, rtmath::ray(rec.p, scatter_direction)};
  }

  rtmath::color albedo;
};

class metal : public material {
public:
  metal(const rtmath::color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual std::optional<scatter_record>
  scatter(const rtmath::ray &r_in, const hit_record &rec) const {
    rtmath::vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    rtmath::ray scattered(
        rec.p, reflected + fuzz * rtmath::random_in_unit_sphere<double, 3>());
    if (dot(scattered.direction(), rec.normal) <= 0)
      return std::nullopt;
    return scatter_record{albedo, scattered};
  }

  rtmath::color albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double ri) : ref_idx(ri) {}

  virtual std::optional<scatter_record>
  scatter(const rtmath::ray &r_in, const hit_record &rec) const {
    rtmath::color attenuation(1.0, 1.0, 1.0);
    double etai_over_etat;
    if (rec.front_face) {
      etai_over_etat = 1.0 / ref_idx;
    } else {
      etai_over_etat = ref_idx;
    }

    rtmath::vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      rtmath::vec3 reflected = reflect(unit_direction, rec.normal);
      return scatter_record{attenuation, rtmath::ray(rec.p, reflected)};
    }
    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (rtmath::random_double() < reflect_prob) {
      rtmath::vec3 reflected = reflect(unit_direction, rec.normal);
      return scatter_record{attenuation, rtmath::ray(rec.p, reflected)};
    }

    rtmath::vec3 refracted =
        refract(unit_direction, rec.normal, etai_over_etat);
    return scatter_record{attenuation, rtmath::ray(rec.p, refracted)};
  }

  double ref_idx;
};
} // namespace rtcore
