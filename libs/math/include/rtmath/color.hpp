#pragma once

#include "vec.hpp"

#include <iostream>
#include <ostream>

inline void write_color(std::ostream &out, rtmath::color pixel_color) {
  // 各成分を [0,255] に変換して出力する
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
