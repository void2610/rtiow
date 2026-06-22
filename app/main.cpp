#include <iostream>
#include <memory>
#include <rtcore/camera.hpp>
#include <rtcore/hittable.hpp>
#include <rtcore/hittable_list.hpp>
#include <rtcore/material.hpp>
#include <rtcore/sphere.hpp>
#include <rtmath/color.hpp>
#include <rtmath/ray.hpp>
#include <rtmath/utils.hpp>
#include <rtmath/vec.hpp>

using namespace rtmath;
using namespace rtcore;
using std::make_shared;

// レイの動きを演算して色を返す。当たらなかったら白-水色のグラデーション(背景色)を返す
color ray_color(const ray &r, const hittable &world, int depth) {
  // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
  if (depth <= 0)
    return color(0, 0, 0);

  if (auto rec = world.hit(r, 0.001, infinity)) {
    if (auto sr = rec->mat_ptr->scatter(r, rec.value()))
      return sr->attenuation * ray_color(sr->scattered, world, depth - 1);
    return color(0, 0, 0);
  }

  vec3 unit_direction = rtmath::unit_vector(r.direction());
  auto t = 0.5 * unit_direction.y() + 1.0;
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<material> sphere_material;
        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

int main() {
  const int image_width = 384;
  const double aspect_ratio = 16.0 / 9.0;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 20;
  const int max_depth = 50;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  auto dist_to_focus = 10;
  auto aperture = 0.1;
  camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

  auto world = random_scene();

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
