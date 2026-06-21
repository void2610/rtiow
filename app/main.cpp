#include <iostream>
#include <memory>
#include <rtcore/camera.hpp>
#include <rtcore/hittable.hpp>
#include <rtcore/hittable_list.hpp>
#include <rtcore/sphere.hpp>
#include <rtmath/color.hpp>
#include <rtmath/ray.hpp>
#include <rtmath/utils.hpp>
#include <rtmath/vec.hpp>

using namespace rtmath;
using namespace rtcore;

// レイの動きを演算して色を返す。当たらなかったら白-水色のグラデーション(背景色)を返す
color ray_color(const ray &r, const hittable &world, int depth) {
  hit_record rec;

  // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
  if (depth <= 0)
    return color(0, 0, 0);

  if (world.hit(r, 0.001, infinity, rec)) {
    point3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
  }

  vec3 unit_direction = rtmath::unit_vector(r.direction());
  auto t = 0.5 * unit_direction.y() + 1.0;
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / (16.0 / 9.0));
  const int samples_per_pixel = 20;
  const int max_depth = 50;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  camera cam;

  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remainning: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        auto r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "\nDone.\n";
}
