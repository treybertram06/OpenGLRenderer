cmake_minimum_required(VERSION 3.30)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

# Include headers
include_directories(include/GLFW)
include_directories(include)

# Platform-specific library paths
if(APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/dependencies/lib-arm64)
    link_directories(${CMAKE_SOURCE_DIR}/dependencies/assimp/lib-arm64)
elseif(WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/dependencies/lib-mingw-w64)
    link_directories(${CMAKE_SOURCE_DIR}/dependencies/assimp/Debug)
endif()

# Find OpenGL (needed for both platforms)
find_package(OpenGL REQUIRED)

# Build glad
add_library(glad STATIC src/glad.c include/shader.h
        include/util_funcs.h
        include/mesh.h
        include/model.h)
target_include_directories(glad PUBLIC include)

# Build main executable
add_executable(OpenGL src/main.cpp
        include/camera.h)

# Link platform-specific libraries
if(APPLE)
    target_link_libraries(OpenGL
            glfw
            glad
            assimp
            OpenGL::GL
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreVideo"
    )
elseif(WIN32)
    target_link_libraries(OpenGL
            glfw3
            glad
            assimp
            opengl32
    )
endif()
