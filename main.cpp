#include <iostream>
#include <cmath>
#include <vector>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include "headers/MathUtils.h"
#include "headers/GameObject.h"
#include "headers/PhysicsEngine.h"
#include <random>

const int WIDTH = 800;
const int HEIGHT = 600;

class Triangle {
public:
  Vector2 a, b, c;

  Triangle(Vector2 a, Vector2 b, Vector2 c) :a(a), b(b), c(c) {}

  void draw(SDL_Renderer* renderer, const Vector2& pos) {

    SDL_Vertex vertices[3];

    vertices[0].color = { 255, 0, 0, 255 };
    vertices[0].position = { a.x + pos.x, a.y + pos.y };
    vertices[0].tex_coord = { 0.0f, 0.0f };

    vertices[1].color = { 255, 0, 0, 255 };
    vertices[1].position = { b.x + pos.x, b.y + pos.y };
    vertices[1].tex_coord = { 0.0f, 0.0f };

    vertices[2].color = { 255, 0, 0, 255 };
    vertices[2].position = { c.x + pos.x, c.y + pos.y };
    vertices[2].tex_coord = { 0.0f, 0.0f };

    SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
  }
};

void drawVector(SDL_Renderer* renderer, const Vector2& v, const Vector2& pos) {
  int x1 = (int)pos.x;
  int y1 = (int)pos.y;
  int x2 = (int)(pos.x + v.x);
  int y2 = (int)(pos.y + v.y);

  float angle = atan2f(v.y, v.x);

  float arrow_angle = 0.5f;
  float arrow_length = 20.0f;

  int x3 = x2 - (int)(arrow_length * cos(angle - arrow_angle));
  int y3 = y2 - (int)(arrow_length * sin(angle - arrow_angle));

  int x4 = x2 - (int)(arrow_length * cos(angle + arrow_angle));
  int y4 = y2 - (int)(arrow_length * sin(angle + arrow_angle));

  for (int i = 0; i < 5; i++) {
    SDL_RenderDrawLine(renderer, x1, y1 + i, x2, y2 + i);
    SDL_RenderDrawLine(renderer, x2, y2 + i, x3, y3 + i);
    SDL_RenderDrawLine(renderer, x2, y2 + i, x4, y4 + i);
  }
}

void drawCircle(SDL_Renderer* renderer, int cx, int cy, int radius) {
  for (int y = -radius; y <= radius; y++) {
    int xSpan = (int)sqrt(radius * radius - y * y);

    SDL_RenderDrawLine(renderer, cx - xSpan, cy + y, cx + xSpan, cy + y);
  }
}

void drawPolygon(SDL_Renderer* renderer, const std::vector<Vector2>& vertices, const Vector2& pos) {
  size_t n = vertices.size();

  for (size_t i = 1; i < n - 1; i++) {

    Triangle triangle(vertices[0], vertices[i], vertices[i + 1]);
    triangle.draw(renderer, pos);
  }
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Physics Engine Test",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WIDTH, HEIGHT, SDL_WINDOW_SHOWN
  );
  PhysicsEngine engine;

  const Vector2 gravity = Vector2(0.0f, 300.0f);
  std::vector<Vector2> vertices = {
    { 0.0f, -40.0f },
    { 35.0f, -20.0f },
    { 35.0f, 20.0f },
    { 0.0f, 40.0f },
    { -35.0f, 20.0f },
    { -35.0f, -20.0f }
  };

  std::vector<Vector2> rect = {
    Vector2(-400, -50),  // top-left
    Vector2(400, -50),  // top-right
    Vector2(400,  50),  // bottom-right
    Vector2(-400,  50)   // bottom-left
  };

  std::vector<GameObject> objects;

  GameObject obj;

  obj.rb.pos = Vector2(400.0f, 300.0f);
  obj.collider = std::make_unique<CircleCollider>(50.0f);

  objects.push_back(std::move(obj));

  GameObject ground;
  ground.rb.isStatic = true;
  ground.rb.inv_mass = 0.0f;
  ground.rb.pos = Vector2(400.0f, 550.0f);
  ground.collider = std::make_unique<PolygonCollider>(rect);

  objects.push_back(std::move(ground));

  if (!window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool quit = false;
  SDL_Event e;

  Uint64 lastTime = SDL_GetTicks64();
  const float fixedDeltaTime = 0.02f;

  float accumulator = 0.0f;
  while (!quit) {
    //Izračunava deltatime
    Uint64 currentTime = SDL_GetTicks64();

    float dt = (float)(currentTime - lastTime) / 1000.0f;

    lastTime = currentTime;

    //Pronalazi sve event-ove
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) quit = true;
    }

    //Ažuriranje tijela
    accumulator += dt;
    while (accumulator >= fixedDeltaTime) {
      for (auto& obj : objects) {
        obj.rb.AddForce(gravity);
        engine.integrate(obj.rb, fixedDeltaTime);
      }
      accumulator -= fixedDeltaTime;
    }

    //Detektiranje sudara između tijela
    engine.detectCollisions(objects);

    //Crtanje tijela
    SDL_SetRenderDrawColor(renderer, 0, 6, 177, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (const auto& obj : objects) {
      if (obj.collider->type == Collider::Circle) {
        CircleCollider* circle = static_cast<CircleCollider*>(obj.collider.get());
        drawCircle(renderer, (int)obj.rb.pos.x, (int)obj.rb.pos.y, (int)obj.GetRadius());
      }
      else if (obj.collider->type == Collider::Polygon) {

        PolygonCollider* polygon = static_cast<PolygonCollider*>(obj.collider.get());
        drawPolygon(renderer, polygon->vertices, obj.rb.pos);
      }
    }
    SDL_RenderPresent(renderer);

  }

  //Pri završetku uništava prozor
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}