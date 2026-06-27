#pragma once

#include "rtmath/ray.hpp"
#include "rtmath/vec.hpp"

namespace rtmath {
class aabb {
public:
  aabb() {}
  aabb(const rtmath::point3 &a, const rtmath::point3 &b) {
    _min = a;
    _max = b;
  }

  rtmath::point3 min() const { return _min; }
  rtmath::point3 max() const { return _max; }

  bool hit(const ray &r, double tmin, double tmax) const;
  rtmath::point3 _min;
  rtmath::point3 _max;
};

// 最適化されたhit実装
inline bool aabb::hit(const ray &r, double tmin, double tmax) const {
  for (int a = 0; a < 3; a++) {
    auto invD = 1.0f / r.direction()[a];
    auto t0 = (min()[a] - r.origin()[a]) * invD;
    auto t1 = (max()[a] - r.origin()[a]) * invD;
    if (invD < 0.0f)
      std::swap(t0, t1);
    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;
    if (tmax <= tmin)
      return false;
  }
  return true;
}

// hitの単純な実装
// inline bool aabb::hit(const ray &r, double tmin, double tmax) const {
//   for (int a = 0; a < 3; a++) {
//     auto t0 = fmin((_min[a] - r.origin()[a]) / r.direction()[a],
//                    (_max[a] - r.origin()[a]) / r.direction()[a]);
//     auto t1 = fmax((_min[a] - r.origin()[a]) / r.direction()[a],
//                    (_max[a] - r.origin()[a]) / r.direction()[a]);
//
//     tmin = fmax(t0, tmin);
//     tmax = fmin(t1, tmax);
//     if (tmax <= tmin)
//       return false;
//   }
//   return true;
// }

inline rtmath::aabb surrounding_box(rtmath::aabb box0, rtmath::aabb box1) {
  rtmath::point3 small(fmin(box0.min().x(), box1.min().x()),
                       fmin(box0.min().y(), box1.min().y()),
                       fmin(box0.min().z(), box1.min().z()));
  rtmath::point3 big(fmin(box0.max().x(), box1.max().x()),
                     fmin(box0.max().y(), box1.max().y()),
                     fmin(box0.max().z(), box1.max().z()));

  return rtmath::aabb(small, big);
}

} // namespace rtmath
