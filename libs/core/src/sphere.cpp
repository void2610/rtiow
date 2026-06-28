#include <cmath>
#include <optional>
#include <rtcore/hittable.hpp>
#include <rtcore/sphere.hpp>
#include <rtmath/aabb.hpp>

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
      auto outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      auto uv = get_sphere_uv(outward_normal);
      rec.u = uv.e[0];
      rec.v = uv.e[1];
      rec.mat_ptr = mat_ptr;
      return rec;
    }
    temp = (-half_b + root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      auto outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      auto uv = get_sphere_uv(outward_normal);
      rec.u = uv.e[0];
      rec.v = uv.e[1];
      rec.mat_ptr = mat_ptr;
      return rec;
    }
  }
  return std::nullopt;
}

std::optional<rtmath::aabb> sphere::bounding_box(double t0, double t1) const {
  auto res = rtmath::aabb(center - vec3(radius, radius, radius),
                          center + vec3(radius, radius, radius));
  return res;
}
} // namespace rtcore
