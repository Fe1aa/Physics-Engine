#include "headers/GameObject.h"

GameObject::GameObject() : rb(), collider(nullptr) {}

float GameObject::GetRadius() const {
  if (!collider || collider->type != Collider::Circle)
    return 0.0f;
  CircleCollider* circle = static_cast<CircleCollider*>(collider.get());
  return circle->radius;
}
