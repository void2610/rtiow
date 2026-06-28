#pragma once

#include "rtmath/perlin.hpp"
#include "rtmath/vec.hpp"
#include <cstddef>
namespace rtcore {
class texture {
public:
  virtual ~texture() {}
  virtual rtmath::color value(double u, double v,
                              const rtmath::point3 &p) const = 0;
};

class solid_color : public texture {
public:
  solid_color() {}
  solid_color(rtmath::color c) : color_value(c) {}

  solid_color(double r, double g, double b)
      : solid_color(rtmath::color(r, g, b)) {}

  virtual rtmath::color value(double u, double v,
                              const rtmath::point3 &p) const {
    return color_value;
  }

private:
  rtmath::color color_value;
};

class checker_texture : public texture {
public:
  checker_texture() {}
  checker_texture(std::shared_ptr<texture> t0, std::shared_ptr<texture> t1)
      : even(t0), odd(t1) {}

  virtual rtmath::color value(double u, double v,
                              const rtmath::point3 &p) const {
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
      return odd->value(u, v, p);
    else
      return even->value(u, v, p);
  }

public:
  std::shared_ptr<texture> even;
  std::shared_ptr<texture> odd;
};

class noise_texture : public texture {
public:
  noise_texture(double sc) : scale(sc) {}

  virtual rtmath::color value(double u, double v,
                              const rtmath::point3 &p) const {
    return rtmath::color(1, 1, 1) * 0.5 *
           (1.0 + sin(scale * p.z() + 10 * noise.turb(p)));
  }

public:
  rtmath::perlin noise;
  double scale;
};

class image_texture : public texture {
public:
  const static int bytes_per_pixel = 3;

  image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

  image_texture(const char *filepath);

  ~image_texture() { delete data; }

  virtual rtmath::color value(double u, double v, const rtmath::vec3 &p) const;

private:
  unsigned char *data;
  int width;
  int height;
  int bytes_per_scanline;
};

} // namespace rtcore
