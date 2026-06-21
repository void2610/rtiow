#pragma once

#include <memory>
#include <rtcore/hittable.hpp>

namespace rtcore {
class sphere : public hittable {
public:
  sphere() {}
  sphere(rtmath::point3 cen, double r, std::shared_ptr<material> m)
      : center(cen), radius(r), mat_ptr(m) {}

  virtual bool hit(const rtmath::ray &r, double t_min, double t_max,
                   hit_record &rec) const;

  rtmath::point3 center;
  double radius;
  std::shared_ptr<material> mat_ptr;
};
} // namespace rtcore
