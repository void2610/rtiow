#include "rtcore/hittable.hpp"
#include "rtcore/material.hpp"
#include "rtmath/vec.hpp"
#include <memory>
#include <optional>

namespace rtcore {

class moving_sphere : public hittable {
public:
  moving_sphere() {}
  moving_sphere(rtmath::point3 cen0, rtmath::point3 cen1, double t0, double t1,
                double r, std::shared_ptr<material> m)
      : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r),
        mat_ptr(m) {}

  virtual std::optional<hit_record> hit(const rtmath::ray &r, double t_min,
                                        double t_max) const;

  rtmath::point3 center(double time) const;

  rtmath::point3 center0;
  rtmath::point3 center1;
  double time0;
  double time1;
  double radius;
  std::shared_ptr<material> mat_ptr;
};

inline rtmath::point3 moving_sphere::center(double time) const {
  return center0 + ((time - time0) / (time1 - time0) * (center1 - center0));
}

inline std::optional<hit_record>
moving_sphere::hit(const rtmath::ray &r, double t_min, double t_max) const {
  hit_record rec;
  rtmath::vec3 oc = r.origin() - center(r.time());
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {
    auto root = sqrt(discriminant);

    auto temp = (-half_b - root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      auto outward_normal = (rec.p - center(r.time())) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat_ptr = mat_ptr;
      return rec;
    }

    temp = (-half_b + root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      auto outward_normal = (rec.p - center(r.time())) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat_ptr = mat_ptr;
      return rec;
    }
  }
  return std::nullopt;
}

} // namespace rtcore
