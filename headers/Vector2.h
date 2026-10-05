#ifndef VECTOR2_H
#define VECTOR2_H
#include <algorithm>
#include <iostream>
class Vector2 {
public:
  float x;
  float y;

  Vector2();
  Vector2(float nx, float ny);

  Vector2 operator+(const Vector2& v) const;
  Vector2 operator-(const Vector2& v) const;
  Vector2 operator*(const float scalar) const;
  Vector2 operator/(const float scalar) const;

  Vector2& operator+=(const Vector2& v);
  Vector2& operator-=(const Vector2& v);
  Vector2& operator*=(const float scalar);
  Vector2& operator/=(const float scalar);

  static float cross(const Vector2& a, const Vector2& b);
  static float dot(const Vector2& a, const Vector2& b);
  float magnitude() const;
  float magnitude2() const;
  void normalize();
  Vector2 normalized() const;
  static Vector2 rotate(const Vector2& v, float angle);
  static Vector2 moveTowards(const Vector2& current, const Vector2& target, float maxDistance);
  static Vector2 rotateTowards(const Vector2& current, const Vector2& target, float maxAngle);

  void debug() const;
};

inline Vector2 operator*(float s, const Vector2& v);
#endif