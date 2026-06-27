#include "rtcore/bvh.hpp"
#include "rtcore/hittable.hpp"
#include "rtmath/aabb.hpp"
#include "rtmath/utils.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

namespace rtcore {
inline bool box_compare(const std::shared_ptr<hittable> a,
                        const std::shared_ptr<hittable> b, int axis) {
  rtmath::aabb box_a;
  rtmath::aabb box_b;

  if (!a->bounding_box(0, 0) || !b->bounding_box(0, 0))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().e[axis] < box_b.min().e[axis];
}

inline bool box_x_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b) {
  return box_compare(a, b, 0);
}
inline bool box_y_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b) {
  return box_compare(a, b, 1);
}
inline bool box_z_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b) {
  return box_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>> &objects,
                   size_t start, size_t end, double time0, double time1) {
  auto axis = rtmath::random_value<int>(0, 2);
  auto comparator = (axis == 0)   ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
    left = right = objects[start];
  } else if (object_span == 2) {
    if (comparator(objects[start], objects[start + 1])) {
      left = objects[start];
      right = objects[start + 1];
    } else {
      left = objects[start + 1];
      right = objects[start];
    }
  } else {
    std::sort(objects.begin() + start, objects.begin() + end, comparator);

    auto mid = start + object_span / 2;
    left = std::make_shared<bvh_node>(objects, start, mid, time0, time1);
    right = std::make_shared<bvh_node>(objects, mid, end, time0, time1);
  }

  rtmath::aabb box_left;
  rtmath::aabb box_right;

  if (!left->bounding_box(time0, time1) || !right->bounding_box(time0, time1))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  box = rtmath::surrounding_box(box_left, box_right);
}
} // namespace rtcore
