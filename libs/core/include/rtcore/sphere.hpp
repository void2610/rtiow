#pragma once

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

  rtmath::point3 center;
  double radius;
  std::shared_ptr<material> mat_ptr;
};
} // namespace rtcore
