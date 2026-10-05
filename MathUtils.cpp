#include "headers/MathUtils.h"


float degreesToRadians(float degrees) {
  return degrees * PI / 180.0f;
}

Vector2 fromAngle(float radians) {
  Vector2 direction;
  direction.x = cos(radians);
  direction.y = -sin(radians);

  return direction;
}
