#include "headers/Collision.h"

Collider::~Collider() = default;

CircleCollider::CircleCollider(float r) : radius(r) {
  type = Circle;
}

PolygonCollider::PolygonCollider(const std::vector<Vector2>& v) : vertices(v), num_vertices(v.size()) {
  type = Polygon;
}
