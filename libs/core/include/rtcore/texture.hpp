#pragma once

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
} // namespace rtcore
