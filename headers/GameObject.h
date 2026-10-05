#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Rigidbody2d.h"
#include "Collision.h"
#include <SDL.h>
#include <memory>


class GameObject {
public:
  RigidBody2D rb;

  std::unique_ptr<Collider> collider;

  GameObject();
  float GetRadius() const;
};

#endif