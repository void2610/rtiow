#include "rtcore/texture.hpp"
#include "rtmath/utils.hpp"
#include "rtmath/vec.hpp"
#include <iostream>
#include <stb_image.h>

namespace rtcore {
image_texture::image_texture(const char *filepath) {
  auto components_per_pixel = bytes_per_pixel;

  data = stbi_load(filepath, &width, &height, &components_per_pixel,
                   components_per_pixel);
  if (!data) {
    std::cerr << "ERROR: Could not load texture image file '" << filepath
              << "'.\n";
    width = 0;
    height = 0;
  }

  bytes_per_scanline = bytes_per_pixel * width;
}

rtmath::color image_texture::value(double u, double v,
                                   const rtmath::vec3 &p) const {
  // テクスチャがない場合、紫色を返す
  if (data == nullptr)
    return rtmath::color(1, 0, 1);

  // 入力されたテクスチャ座標を[0,1]で切り捨てる
  u = rtmath::clamp(u, 0.0, 1.0);
  v = 1.0 - rtmath::clamp(v, 0.0, 1.0);

  auto i = static_cast<int>(u * width);
  auto j = static_cast<int>(v * height);

  // 整数座標をさらに切り捨てる (テクスチャ座標は1.0になってはいけない)
  if (i >= width)
    i = width - 1;
  if (j >= height)
    j = height - 1;

  const auto color_scale = 1.0 / 255.0;
  auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
  return rtmath::color(color_scale * pixel[0], color_scale * pixel[1],
                       color_scale * pixel[2]);
}
} // namespace rtcore
