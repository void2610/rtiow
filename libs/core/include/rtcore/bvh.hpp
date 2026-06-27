#pragma once

#include "rtcore/hittable.hpp"
#include "rtcore/hittable_list.hpp"
#include "rtmath/aabb.hpp"
#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace rtcore {
class bvh_node : public rtcore::hittable {
public:
  bvh_node() {}
  bvh_node(rtcore::hittable_list &list, double time0, double time1)
      : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}
  bvh_node(std::vector<std::shared_ptr<hittable>> &objects, size_t start,
           size_t end, double time0, double time1);

  virtual std::optional<rtcore::hit_record>
  hit(const rtmath::ray &r, double t_min, double t_max) const;
  virtual std::optional<rtmath::aabb> bounding_box(double t0, double t1) const;

  std::shared_ptr<hittable> left;
  std::shared_ptr<hittable> right;
  rtmath::aabb box;
};

inline std::optional<rtcore::hit_record>
bvh_node::hit(const rtmath::ray &r, double t_min, double t_max) const {
  if (!box.hit(r, t_min, t_max))
    return std::nullopt;

  auto hit_left = left->hit(r, t_min, t_min);
  auto hit_right = right->hit(r, t_min, hit_left ? hit_left->t : t_max);

  return hit_right;
}

inline std::optional<rtmath::aabb> bvh_node::bounding_box(double t0,
                                                          double t1) const {
  return box;
}
} // namespace rtcore
