#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <cmath>
#include "Vector2.h"


inline constexpr float PI = 3.141592653589793f;

float degreesToRadians(float degrees);

Vector2 fromAngle(float radians);

template<typename T>
T lerp(const T& A, const T& B, float t) {
  t = std::clamp(t, 0.0f, 1.0f);

  return A + (B - A) * t;
}


#endif