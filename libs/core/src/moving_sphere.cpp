#include "rtcore/moving_sphere.hpp"
#include "rtcore/hittable.hpp"
#include "rtmath/aabb.hpp"
#include "rtmath/vec.hpp"
#include <memory>

namespace rtcore {

std::optional<hit_record> moving_sphere::hit(const rtmath::ray &r, double t_min,
                                             double t_max) const {
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

std::optional<rtmath::aabb> moving_sphere::bounding_box(double t0,
                                                        double t1) const {
  auto box0 = rtmath::aabb(center(t0) - rtmath::vec3(radius, radius, radius),
                           center(t0) + rtmath::vec3(radius, radius, radius));
  auto box1 = rtmath::aabb(center(t1) - rtmath::vec3(radius, radius, radius),
                           center(t1) + rtmath::vec3(radius, radius, radius));
  return surrounding_box(box0, box1);
}

} // namespace rtcore
