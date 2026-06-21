#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
public:
  hittable_list() {}
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(shared_ptr<hittable> object) { objects.push_back(object); }

  virtual bool hit(const ray &r, double tmin, double tmax,
                   hit_record &rec) const;

  std::vector<shared_ptr<hittable>> objects;
};
