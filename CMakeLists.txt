cmake_minimum_required(VERSION 3.30)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/GLFW)
include_directories(include)

link_directories(dependencies/lib-mingw-w64)
find_package(OpenGL REQUIRED)

add_library(glad STATIC src/glad.c
        include/shader.h)
target_include_directories(glad PUBLIC include)

add_executable(OpenGL src/main.cpp)

target_link_libraries(OpenGL glfw3 opengl32 glad)
