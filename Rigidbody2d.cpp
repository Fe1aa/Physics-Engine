#include "headers/Rigidbody2d.h"

RigidBody2D::RigidBody2D()
  : pos(0, 0),
  linear_velocity(0, 0),
  force(0, 0),
  angular_velocity(0.0f),
  orientation(0.0f),
  mass(1.0f),
  inv_mass(1.0f),
  friction(0.5f),
  restitution(1.0f),
  inertia(1.0f),
  inv_inertia(1.0f),
  torque(0.0f),
  isStatic(false) {
}

void RigidBody2D::SetMass(float m) {
  mass = m;
  inv_mass = (m > 0.0f && !isStatic) ? 1.0f / m : 0.0f;
}

void RigidBody2D::SetInertia(float i) {
  inertia = i;
  inv_inertia = (i > 0.0f && !isStatic) ? 1.0f / i : 0.0f;
}

void RigidBody2D::AddForce(const Vector2& f) {
  if (inv_mass == 0.0f) return;
  force += f;
}

void RigidBody2D::AddTorque(float t) {
  if (inv_inertia == 0.0f) return;
  torque += t;
}

void RigidBody2D::ResetForce() {
  force = Vector2(0, 0);
  torque = 0;
}
