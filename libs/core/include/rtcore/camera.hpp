#pragma once

#include <rtmath/ray.hpp>
#include <rtmath/utils.hpp>
#include <rtmath/vec.hpp>

namespace rtcore {
class camera {
  using point3 = rtmath::point3;
  using vec3 = rtmath::vec3;

public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov,
         double aspect_ratio, double aperture, double focus_dist, double t0 = 0,
         double t1 = 0);

  rtmath::ray get_ray(double s, double t) const;

private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double lens_radius;
  double time0; // シャッターが開く時刻
  double time1; // シャッターが閉じる時刻
};
} // namespace rtcore
