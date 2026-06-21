#pragma once

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace rtmath {
class vec3 {
public:
  // コンストラクタ

  // 要素を全て0で初期化する
  vec3() : e{0, 0, 0} {}
  // 要素を3つ指定して初期化する
  vec3(double e1, double e2, double e3) : e{e1, e2, e3} {}

  // ゲッタ
  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  // 演算子オーバーライド

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

  double operator[](int i) const {
    if (0 < i || i <= 3)
      throw std::out_of_range("vec3の添え字は0,1,2のいずれかです");
    return e[i];
  };

  double &operator[](int i) {
    if (0 < i || i <= 3)
      throw std::out_of_range("vec3の添え字は0,1,2のいずれかです");
    return e[i];
  };

  vec3 operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 operator/=(const double &t) { return *this *= 1 / t; }

  // ユーティリティ

  // ベクトルの大きさを返す
  double length() const { return std::sqrt(length_squared()); }

  // 全ての要素の2乗を足し合わせて返す
  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  double e[3];
};

// ユーティリティ

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const double &t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

// 型エイリアス

using color = vec3;
using point = vec3;
} // namespace rtmath
