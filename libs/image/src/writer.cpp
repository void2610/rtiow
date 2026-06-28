#include <rtimage/writer.hpp>

#include <cmath>
#include <fstream>
#include <rtmath/utils.hpp>
#include <stb_image_write.h>

namespace rtimage {

image::image(int width, int height)
    : width_(width), height_(height),
      pixels_(static_cast<std::size_t>(width) * height * 3, 0) {}

void image::set_pixel(int x, int y, const rtmath::color &c,
                      int samples_per_pixel) {
  auto r = c.x();
  auto g = c.y();
  auto b = c.z();

  // サンプル平均 + gamma=2.0 のガンマ補正
  auto scale = 1.0 / samples_per_pixel;
  r = std::sqrt(scale * r);
  g = std::sqrt(scale * g);
  b = std::sqrt(scale * b);

  const std::size_t idx = (static_cast<std::size_t>(y) * width_ + x) * 3;
  pixels_[idx + 0] =
      static_cast<unsigned char>(255.999 * rtmath::clamp(r, 0.0, 0.999));
  pixels_[idx + 1] =
      static_cast<unsigned char>(255.999 * rtmath::clamp(g, 0.0, 0.999));
  pixels_[idx + 2] =
      static_cast<unsigned char>(255.999 * rtmath::clamp(b, 0.0, 0.999));
}

bool image::save(const std::string &path, format fmt) const {
  switch (fmt) {
  case format::png:
    return stbi_write_png(path.c_str(), width_, height_, 3, pixels_.data(),
                          width_ * 3) != 0;
  case format::ppm: {
    std::ofstream ofs(path);
    if (!ofs)
      return false;
    ofs << "P3\n" << width_ << ' ' << height_ << "\n255\n";
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        const std::size_t idx = (static_cast<std::size_t>(y) * width_ + x) * 3;
        ofs << static_cast<int>(pixels_[idx + 0]) << ' '
            << static_cast<int>(pixels_[idx + 1]) << ' '
            << static_cast<int>(pixels_[idx + 2]) << '\n';
      }
    }
    return static_cast<bool>(ofs);
  }
  }
  return false;
}
} // namespace rtimage
