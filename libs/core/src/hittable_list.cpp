#include "rtmath/aabb.hpp"
#include <optional>
#include <rtcore/hittable_list.hpp>

namespace rtcore {
std::optional<hit_record> hittable_list::hit(const rtmath::ray &r, double t_min,
                                             double t_max) const {
  std::optional<hit_record> result;
  auto closest_so_far = t_max;

  for (const auto &object : objects) {
    if (const auto rec = object->hit(r, t_min, closest_so_far)) {
      closest_so_far = rec->t;
      result = rec;
    }
  }

  return result;
}

std::optional<rtmath::aabb> hittable_list::bounding_box(double t0,
                                                        double t1) const {
  if (objects.empty())
    return std::nullopt;

  rtmath::aabb res;
  rtmath::aabb temp;
  bool first_box = true;

  for (const auto &object : objects) {
    if (!object->bounding_box(t0, t1))
      return std::nullopt;
    res = first_box ? temp : rtmath::surrounding_box(res, temp);
    first_box = false;
  }

  return res;
}
} // namespace rtcore
