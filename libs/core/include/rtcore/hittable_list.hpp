#pragma once

#include <rtcore/hittable.hpp>

#include <memory>
#include <vector>

namespace rtcore {
class hittable_list : public hittable {
public:
  hittable_list() {}
  hittable_list(std::shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

  virtual std::optional<hit_record> hit(const rtmath::ray &r, double t_min,
                                        double t_max) const;

  std::vector<std::shared_ptr<hittable>> objects;
};
} // namespace rtcore
