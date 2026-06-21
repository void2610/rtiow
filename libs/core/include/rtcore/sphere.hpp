#pragma once

#include <rtcore/hittable.hpp>

namespace rtcore {
class sphere : public hittable {
public:
  sphere() {}
  sphere(rtmath::point3 cen, double r) : center(cen), radius(r) {}

  virtual bool hit(const rtmath::ray &r, double t_min, double t_max,
                   hit_record &rec) const;

  rtmath::point3 center;
  double radius;
};
} // namespace rtcore
