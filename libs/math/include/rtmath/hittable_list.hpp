#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

namespace rtmath {
class hittable_list : public hittable {
public:
  hittable_list() {}
  hittable_list(std::shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

  virtual bool hit(const ray &r, double tmin, double tmax,
                   hit_record &rec) const;

  std::vector<std::shared_ptr<hittable>> objects;
};
} // namespace rtmath
