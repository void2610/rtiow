#pragma once

#include "rtmath/utils.hpp"
#include "rtmath/vec.hpp"
#include <memory>
#include <rtcore/hittable.hpp>

namespace rtcore {
class sphere : public hittable {
public:
  sphere() {}
  sphere(rtmath::point3 cen, double r, std::shared_ptr<material> m)
      : center(cen), radius(r), mat_ptr(m) {}

  virtual std::optional<hit_record> hit(const rtmath::ray &r, double t_min,
                                        double t_max) const;
  virtual std::optional<rtmath::aabb> bounding_box(double t0, double t1) const;

  rtmath::point3 center;
  double radius;
  std::shared_ptr<material> mat_ptr;
};

inline rtmath::vec<double, 2> get_sphere_uv(const rtmath::vec3 &p) {
  auto phi = atan2(p.z(), p.x());
  auto theta = asin(p.y());
  rtmath::vec<double, 2> res;
  res.e[0] = 1 - (phi + rtmath::pi) / (2 * rtmath::pi);
  res.e[1] = (theta + rtmath::pi / 2) / rtmath::pi;
  return res;
}
} // namespace rtcore
