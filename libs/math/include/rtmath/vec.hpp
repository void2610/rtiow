#pragma once

#include "rtmath/utils.hpp"
#include <array>
#include <cmath>
#include <concepts>
#include <ostream>
#include <stdexcept>

namespace rtmath {

template <std::floating_point T, std::size_t N> class vec {
public:
  // コンストラクタ

  // 要素を全て0で初期化する
  vec() : e{} {}
  // 要素を3つ指定して初期化する
  vec(T e1, T e2, T e3) : e{e1, e2, e3} {}
  // 要素の配列から初期化する
  vec(const std::array<T, N> &v) : e(v) {}

  // ゲッタ
  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  inline static vec random() {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r.e[i] = random_value<T>();
    }
    return r;
  }

  inline static vec random(double min, double max) {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r.e[i] = random_value<T>(min, max);
    }
    return r;
  }

  // 演算子オーバーライド

  vec operator-() const {
    vec r;
    for (std::size_t i = 0; i < N; i++) {
      r.e[i] = -e[i];
    }
    return r;
  }

  double operator[](std::size_t i) const {
    if (N <= i)
      throw std::out_of_range("vecの大きさ外にアクセスしています");
    return e[i];
  };

  double &operator[](std::size_t i) {
    if (N <= i)
      throw std::out_of_range("vecの大きさ外にアクセスしています");
    return e[i];
  };

  vec &operator+=(const vec &v) {
    for (std::size_t i = 0; i < N; i++) {
      e[i] += v.e[i];
    }
    return *this;
  }

  vec &operator*=(const double t) {
    for (std::size_t i = 0; i < N; i++) {
      e[i] *= t;
    }
    return *this;
  }

  vec &operator/=(const double &t) { return *this *= 1 / t; }

  // ユーティリティ

  // ベクトルの大きさを返す
  double length() const { return std::sqrt(length_squared()); }

  // 全ての要素の2乗を足し合わせて返す
  double length_squared() const {
    double l = 0.0;
    for (std::size_t i = 0; i < N; i++) {
      l += e[i] * e[i];
    }
    return l;
  }

  std::array<T, N> e;
};

// ユーティリティ

template <std::floating_point T, std::size_t N>
inline std::ostream &operator<<(std::ostream &out, const vec<T, N> &v) {
  for (std::size_t i = 0; i < N; i++) {
    out << v[i] << ' ';
  }
  return out;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator+(const vec<T, N> &u, const vec<T, N> &v) {
  vec<T, N> r;
  for (std::size_t i = 0; i < N; i++) {
    r.e[i] = u[i] + v[i];
  }
  return r;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator-(const vec<T, N> &u, const vec<T, N> &v) {
  vec<T, N> r;
  for (std::size_t i = 0; i < N; i++) {
    r.e[i] = u[i] - v[i];
  }
  return r;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator*(const vec<T, N> &u, const vec<T, N> &v) {
  vec<T, N> r;
  for (std::size_t i = 0; i < N; i++) {
    r.e[i] = u[i] * v[i];
  }
  return r;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator*(const T &t, const vec<T, N> &v) {
  vec<T, N> r;
  for (std::size_t i = 0; i < N; i++) {
    r.e[i] = t * v[i];
  }
  return r;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator*(const vec<T, N> &v, T t) {
  return t * v;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> operator/(const vec<T, N> &v, double t) {
  return (1 / t) * v;
}

template <std::floating_point T, std::size_t N>
inline T dot(const vec<T, N> &u, const vec<T, N> &v) {
  T r = 0.0;
  for (std::size_t i = 0; i < N; i++) {
    r += u[i] * v[i];
  }
  return r;
}

// 外積はN==3の時だけ存在する
template <std::floating_point T, std::size_t N>
  requires(N == 3)
inline vec<T, N> cross(const vec<T, N> &u, const vec<T, N> &v) {
  return vec<T, N>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                   u.e[2] * v.e[0] - u.e[0] * v.e[2],
                   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> unit_vector(vec<T, N> v) {
  return v / v.length();
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> random_in_unit_sphere() {
  while (true) {
    auto p = vec<T, N>::random(-1, 1);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> random_unit_vector() {
  auto a = random_double(0, 2 * pi);
  auto z = random_double(-1, 1);
  auto r = sqrt(1 - z * z);
  return vec<T, N>(r * cos(a), r * sin(a), z);
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> random_in_hemisphere(const vec<T, N> &normal) {
  auto in_unit_sphere = random_in_unit_sphere<T, N>();
  if (dot(in_unit_sphere, normal) > 0.0)
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> random_in_unit_disk() {
  while (true) {
    auto p = vec<T, N>(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> reflect(const vec<T, N> &v, const vec<T, N> &n) {
  return v - 2 * dot(v, n) * n;
}

template <std::floating_point T, std::size_t N>
inline vec<T, N> refract(const vec<T, N> &uv, const vec<T, N> &n,
                         double etai_over_etat) {
  auto cos_theta = dot(-uv, n);
  vec<T, N> r_out_parallel = etai_over_etat * (uv + cos_theta * n);
  vec<T, N> r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
  return r_out_parallel + r_out_perp;
}

// 型エイリアス

using vec3 = vec<double, 3>;
using color = vec<double, 3>;
using point3 = vec<double, 3>;
} // namespace rtmath
