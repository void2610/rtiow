#pragma once

#include "rtmath/perlin.hpp"
#include "rtmath/vec.hpp"
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
  noise_texture() {}

  virtual rtmath::color value(double u, double v,
                              const rtmath::point3 &p) const {
    return rtmath::color(1, 1, 1) * noise.noise(p);
  }

public:
  rtmath::perlin noise;
};

} // namespace rtcore
