#include "headers/Vector2.h"
#include <cmath>

Vector2::Vector2() : x(0.0f), y(0.0f) {}
Vector2::Vector2(float nx, float ny) : x(nx), y(ny) {}


Vector2 Vector2::operator+(const Vector2& v) const {
  return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const {
  return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(float scalar) const {
  return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
  return Vector2(x / scalar, y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector2& Vector2::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Vector2& Vector2::operator/=(const float scalar) {
  x /= scalar;
  y /= scalar;
  return *this;
}

float Vector2::cross(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

float Vector2::dot(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

float Vector2::magnitude() const {
  return sqrt(x * x + y * y);
}

float Vector2::magnitude2() const {
  return x * x + y * y;
}

void Vector2::normalize() {
  float m = magnitude();

  if (m > 0.0f) {
    x /= m;
    y /= m;
  }
}

Vector2 Vector2::normalized() const {
  float m = magnitude();
  if (m > 0.0f) return Vector2(x / m, y / m);
  return Vector2(0, 0);
}

Vector2 Vector2::rotate(const Vector2& v, float angle) {
  float sin_angle = std::sin(angle);
  float cos_angle = std::cos(angle);

  return Vector2(
    cos_angle * v.x - sin_angle * v.y,
    sin_angle * v.x + cos_angle * v.y
  );
}

Vector2 Vector2::moveTowards(const Vector2& current, const Vector2& target, float maxDistance) {
  Vector2 delta = Vector2(target.x - current.x, target.y - current.y);

  float distance2 = delta.magnitude2();

  if (distance2 == 0.0f || (maxDistance >= 0.0f && distance2 <= maxDistance * maxDistance)) {
    return target;
  }

  float distance = std::sqrt(distance2);

  return current + (delta / distance) * maxDistance;
}

Vector2 Vector2::rotateTowards(const Vector2& current, const Vector2& target, float maxAngle) {
  Vector2 a = current.normalized();
  Vector2 b = target.normalized();

  float dot = Vector2::dot(a, b);

  dot = std::clamp(dot, -1.0f, 1.0f);

  float angle = acos(dot);

  float length = current.magnitude();

  if (angle < 0.0001f) {
    return target.normalized() * length;
  }

  float step = std::min(angle, maxAngle);

  float dir = (Vector2::cross(a, b) >= 0.0f) ? 1.0f : -1.0f;

  Vector2 rotatedDir = rotate(a, step * dir);

  return rotatedDir * length;
}

void Vector2::debug() const {
  std::cout << x << " " << y << std::endl;
}

inline Vector2 operator*(float s, const Vector2& v) {
  return Vector2(v.x * s, v.y * s);
}