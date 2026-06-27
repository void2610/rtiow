#pragma once

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

  virtual std::optional<rtmath::aabb> bounding_box(double t0, double t1) const;

  rtmath::point3 center(double time) const {
    return center0 + ((time - time0) / (time1 - time0) * (center1 - center0));
  }

  rtmath::point3 center0;
  rtmath::point3 center1;
  double time0;
  double time1;
  double radius;
  std::shared_ptr<material> mat_ptr;
};
} // namespace rtcore
