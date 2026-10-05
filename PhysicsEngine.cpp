#include "headers/PhysicsEngine.h"
#include <iostream>

void PhysicsEngine::integrate(RigidBody2D& body, float dt) {
  if (body.inv_mass == 0.0f) return;

  Vector2 acceleration = body.force * body.inv_mass;
  body.linear_velocity += acceleration * dt;
  body.pos += body.linear_velocity * dt;

  float angular_accelartion = body.torque * body.inertia;
  body.angular_velocity += angular_accelartion * dt;
  body.orientation += body.angular_velocity * dt;

  body.ResetForce();
}

CollisionManifold PhysicsEngine::circleVsCircle(GameObject& obj1, GameObject& obj2) {
  CollisionManifold m;
  Vector2 dv = obj2.rb.pos - obj1.rb.pos;

  float d2 = dv.magnitude2();

  CircleCollider* c1 = static_cast<CircleCollider*>(obj1.collider.get());
  CircleCollider* c2 = static_cast<CircleCollider*>(obj2.collider.get());

  float r1 = c1->radius;
  float r2 = c2->radius;

  float rsum = r1 + r2;

  if (d2 >= rsum * rsum) {
    return m;
  }

  float d = sqrt(d2);

  m.colliding = true;
  if (d > 0.0f)
  {
    m.normal = dv.normalized();
  }
  else
  {
    m.normal = Vector2(1.0f, 0.0f);
  }
  m.penetration = rsum - d;
  return m;
}

void PhysicsEngine::projectPolygon(const PolygonCollider* collider, const Vector2& pos, const Vector2& axis, float& min, float& max) {
  Vector2 worldPoint = Vector2(collider->vertices[0].x + pos.x, collider->vertices[0].y + pos.y);
  min = Vector2::dot(worldPoint, axis);
  max = min;

  for (size_t i = 1; i < collider->num_vertices; i++) {
    worldPoint = Vector2(collider->vertices[i].x + pos.x, collider->vertices[i].y + pos.y);

    float projection = Vector2::dot(worldPoint, axis);;
    if (projection < min) min = projection;
    if (projection > max) max = projection;
  }
}

void PhysicsEngine::generateAxes(const PolygonCollider* collider, std::vector<Vector2>& axes) {
  for (size_t i = 0; i < collider->num_vertices; i++) {
    Vector2 pos1 = collider->vertices[i];
    Vector2 pos2 = collider->vertices[(i + 1) % collider->num_vertices];

    Vector2 edge = pos2 - pos1;
    Vector2 normal = Vector2(-edge.y, edge.x).normalized();
    axes.push_back(normal);
  }
}

CollisionManifold PhysicsEngine::polygonVsPolygon(GameObject& obj1, GameObject& obj2) {
  CollisionManifold m;

  PolygonCollider* p1 = static_cast<PolygonCollider*>(obj1.collider.get());
  PolygonCollider* p2 = static_cast<PolygonCollider*>(obj2.collider.get());

  size_t total_vertices = p1->num_vertices + p2->num_vertices;

  std::vector<Vector2> axes;
  axes.reserve(total_vertices);

  generateAxes(p1, axes);
  generateAxes(p2, axes);

  float min1, max1;
  float min2, max2;
  for (const auto& axis : axes) {
    projectPolygon(p1, obj1.rb.pos, axis, min1, max1);
    projectPolygon(p2, obj2.rb.pos, axis, min2, max2);

    if (min1 > max2 || min2 > max1) {
      return m;
    }

    float penetration = std::min(max1, max2) - std::max(min1, min2);

    if (penetration < m.penetration) {
      m.penetration = penetration;
      m.normal = axis;
    }
  }

  m.colliding = true;

  Vector2 dir = obj2.rb.pos - obj1.rb.pos;

  if (Vector2::dot(dir, m.normal) < 0.0f) {
    m.normal *= -1.0f;
  }

  return m;
}

CollisionManifold PhysicsEngine::polygonVsCircle(GameObject& obj1, GameObject& obj2) {
  CollisionManifold m;

  PolygonCollider* p;
  CircleCollider* c;

  Vector2 circleCenter;

  if (obj1.collider->type == Collider::Polygon) {
    p = static_cast<PolygonCollider*>(obj1.collider.get());
    c = static_cast<CircleCollider*>(obj2.collider.get());
    circleCenter = obj2.rb.pos;
  }
  else {
    p = static_cast<PolygonCollider*>(obj2.collider.get());
    c = static_cast<CircleCollider*>(obj1.collider.get());
    circleCenter = obj1.rb.pos;
  }

  Vector2 closest;

  for (size_t i = 0; i < p->num_vertices - 1; i++) {
    Vector2 ab = p->vertices[i + 1] - p->vertices[i];

    float t = Vector2::dot(circleCenter - p->vertices[i], ab) / Vector2::dot(ab, ab);

    t = std::clamp(t, 0.0f, 1.0f);

    Vector2 current = p->vertices[i] + ab * t;

    if(current.magnitude2() < closest.magnitude2()) {
      closest = current;
    }
  }

  Vector2 diff = circleCenter - closest;
  
  float dist2 = diff.magnitude2();

  if(dist2 > c->radius * c->radius) {
    return m;
  }

  if(dist2 == 0.0f) {
    
  }
}


CollisionManifold PhysicsEngine::objectsColliding(GameObject& obj1, GameObject& obj2) {
  CollisionManifold m;
  if (obj1.collider->type == Collider::Circle
    && obj2.collider->type == Collider::Circle) {
    m = circleVsCircle(obj1, obj2);
  }
  else if (obj1.collider->type == Collider::Polygon
    && obj2.collider->type == Collider::Polygon) {
    m = polygonVsPolygon(obj1, obj2);
  }
  else if ((obj1.collider->type == Collider::Polygon
    && obj2.collider->type == Collider::Circle) ||
    (obj1.collider->type == Collider::Circle
      && obj2.collider->type == Collider::Polygon)) {

  }
  return m;
}

void PhysicsEngine::detectCollisions(std::vector<GameObject>& objects) {
  size_t size = objects.size();
  for (size_t i = 0; i < size; i++) {
    for (size_t j = i + 1; j < size; j++) {
      if (!objects[i].collider || !objects[j].collider)
        continue;

      CollisionManifold m = objectsColliding(objects[i], objects[j]);


      ResolveCollsion(objects[i].rb, objects[j].rb, m);

    }
  }
}

void PhysicsEngine::ResolveCollsion(RigidBody2D& body1, RigidBody2D& body2, const CollisionManifold& m) {
  if (!m.colliding) {
    return;
  }

  Vector2 relativeVel = body2.linear_velocity - body1.linear_velocity;

  float velAlongNormal = Vector2::dot(relativeVel, m.normal);

  if (velAlongNormal > 0.0f) {
    return;
  }

  float e = std::min(body1.restitution, body2.restitution);

  float invMassSum = body1.inv_mass + body2.inv_mass;

  if (invMassSum <= 0.0f) {
    return;
  }

  float j = -(1 + e) * velAlongNormal;
  j /= invMassSum;

  Vector2 impulse = m.normal * j;

  body1.linear_velocity -= (impulse * body1.inv_mass);
  body2.linear_velocity += (impulse * body2.inv_mass);

  float correct1 = 0.0f;
  float correct2 = 0.0f;

  if (body1.inv_mass > 0.0f) {
    correct1 = m.penetration * (body1.inv_mass / invMassSum);
  }

  if (body2.inv_mass > 0.0f) {
    correct2 = m.penetration * (body2.inv_mass / invMassSum);
  }

  body1.pos -= m.normal * correct1;
  body2.pos += m.normal * correct2;
}