#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H
#include "Vector2.h"

class RigidBody2D
{
public:
  Vector2 pos;
  Vector2 linear_velocity;
  Vector2 force;

  float angular_velocity;
  float orientation;

  float mass;
  float inv_mass;

  float friction;
  float restitution;

  float inertia;
  float inv_inertia;

  float torque;

  bool isStatic;

  RigidBody2D();

  void AddForce(const Vector2& f);
  void AddTorque(float t);
  void SetMass(float m);
  void SetInertia(float i);
  void ResetForce();
};


#endif