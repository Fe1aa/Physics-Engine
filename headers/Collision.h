#ifndef COLLISION_H
#define COLLISION_H
#include "Vector2.h"
#include <vector>

class Collider {
public:
  virtual ~Collider();

  enum Type {
    Circle,
    Polygon
  };

  Type type;
};

class CircleCollider : public Collider {
public:
  float radius;
  CircleCollider(float r);
};

class PolygonCollider : public Collider {
public:
  size_t num_vertices;
  std::vector<Vector2> vertices;
  PolygonCollider(const std::vector<Vector2>& v);
};

#endif
