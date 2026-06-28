#pragma once

#include <concepts>
#include <limits>
#include <random>

namespace rtmath {
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

inline double trilinear_interp(double c[2][2][2], double u, double v,
                               double w) {
  auto accum = 0.0;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++)
        accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                 (k * w + (1 - k) * (1 - w)) * c[i][j][k];
  return accum;
}

template <typename T> inline T random_value() {
  static thread_local std::mt19937 gen{std::random_device{}()};
  if constexpr (std::floating_point<T>) {
    std::uniform_real_distribution<T> dist(T{0}, T{1});
    return dist(gen);
  } else if constexpr (std::integral<T>) {
    std::uniform_int_distribution<T> dist(T{0}, T{1});
    return dist(gen);
  }
}

template <typename T> inline T random_value(T min, T max) {
  static thread_local std::mt19937 gen{std::random_device{}()};
  if constexpr (std::floating_point<T>) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(gen);
  } else if constexpr (std::integral<T>) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(gen);
  }
}

inline double random_double() { return random_value<double>(); }

inline double random_double(double min, double max) {
  return random_value<double>(min, max);
}

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

} // namespace rtmath
