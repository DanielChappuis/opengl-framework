# Minimum cmake version required
cmake_minimum_required(VERSION 2.6)

# Project configuration
PROJECT(OPENGLFRAMEWORKDEMO)

# Headers
INCLUDE_DIRECTORIES(${OPENGLFRAMEWORKDEMO_SOURCE_DIR})

# Copy the torus.obj file used for the demo into the build directory
FILE(COPY "${CMAKE_SOURCE_DIR}/demo/torus.obj" DESTINATION "${CMAKE_BINARY_DIR}/demo/")

# Copy the shaders used for the demo into the build directory
FILE(COPY "${CMAKE_SOURCE_DIR}/src/shaders/" DESTINATION "${CMAKE_BINARY_DIR}/demo/shaders/")

# Create the demo executable
ADD_EXECUTABLE(demo main.cpp Scene.h Scene.cpp)

TARGET_LINK_LIBRARIES(demo openglframework)
