cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)

add_executable(PhysicsEngine main.cpp Vector2.cpp Rigidbody2d.cpp GameObject.cpp MathUtils.cpp Collision.cpp PhysicsEngine.cpp)

target_link_libraries(PhysicsEngine PRIVATE SDL2::SDL2 SDL2::SDL2main)