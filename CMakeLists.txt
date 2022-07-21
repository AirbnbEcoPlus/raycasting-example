cmake_minimum_required(VERSION 3.22)
project(raycasting_example C)

set(CMAKE_C_STANDARD 99)

add_executable(raycasting_example c/main.c)

target_link_libraries(raycasting_example -lglu32 -lglut32 -lopengl32)