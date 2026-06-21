#pragma once

#include <rtmath/ray.hpp>

namespace rtcore {
struct hit_record {
  rtmath::point3 p;
  rtmath::vec3 normal;
  double t;
  bool front_face;

  inline void set_face_normal(const rtmath::ray &r,
                              const rtmath::vec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual ~hittable() {}
  virtual bool hit(const rtmath::ray &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
};
} // namespace rtcore
