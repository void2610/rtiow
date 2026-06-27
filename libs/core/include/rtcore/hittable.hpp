#pragma once

#include <memory>
#include <optional>
#include <rtmath/aabb.hpp>
#include <rtmath/ray.hpp>

namespace rtcore {

class material;

struct hit_record {
  rtmath::point3 p;
  rtmath::vec3 normal;
  std::shared_ptr<material> mat_ptr;
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
  virtual std::optional<hit_record> hit(const rtmath::ray &r, double t_min,
                                        double t_max) const = 0;
  virtual std::optional<rtmath::aabb> bounding_box(double t0,
                                                   double t1) const = 0;
};
} // namespace rtcore
