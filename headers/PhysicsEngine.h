#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "GameObject.h"
#include "Collision.h"
#include <Vector>

struct CollisionManifold {
  bool colliding = false;
  Vector2 normal = Vector2(0.0f, 0.0f);
  float penetration = std::numeric_limits<float>::max();
};

class PhysicsEngine
{
  CollisionManifold objectsColliding(GameObject& obj1, GameObject& obj2);
  CollisionManifold circleVsCircle(GameObject& obj1, GameObject& obj2);
  void projectPolygon(const PolygonCollider* collider, const Vector2& pos, const Vector2& axis, float& min, float& max);
  void generateAxes(const PolygonCollider* collider, std::vector<Vector2>& axes);
  CollisionManifold polygonVsPolygon(GameObject& obj1, GameObject& obj2);
  CollisionManifold polygonVsCircle(GameObject& obj1, GameObject& obj2);
  void ResolveCollsion(RigidBody2D& obj1, RigidBody2D& obj2, const CollisionManifold& m);
public:
  void integrate(RigidBody2D& body, float dt);
  void detectCollisions(std::vector<GameObject>& objects);
};

#endif