#include "rtcore/hittable.hpp"
#include <cmath>
#include <optional>
#include <rtcore/sphere.hpp>

using namespace rtmath;

namespace rtcore {
std::optional<hit_record> sphere::hit(const rtmath::ray &r, double t_min,
                                      double t_max) const {
  hit_record rec;
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {
    auto root = std::sqrt(discriminant);
    auto temp = (-half_b - root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.set_face_normal(r, (rec.p - center) / radius);
      rec.mat_ptr = mat_ptr;
      return rec;
    }
    temp = (-half_b + root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.set_face_normal(r, (rec.p - center) / radius);
      rec.mat_ptr = mat_ptr;
      return rec;
    }
  }
  return std::nullopt;
}
} // namespace rtcore
