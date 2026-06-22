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
} // namespace rtcore
