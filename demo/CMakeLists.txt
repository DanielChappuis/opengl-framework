# Minimum cmake version required
cmake_minimum_required(VERSION 2.6)

# Project configuration
PROJECT(OPENGLFRAMEWORKDEMO)

# Headers
INCLUDE_DIRECTORIES(${OPENGLFRAMEWORKDEMO_SOURCE_DIR})

# Create the demo executable
ADD_EXECUTABLE(demo main.cpp Scene.h Scene.cpp)

TARGET_LINK_LIBRARIES(demo openglframework)

# Copy the shaders folder from sources to build directory
#execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"${CMAKE_SOURCE_DIR}/src/shaders\" \"${CMAKE_CURRENT_BINARY_DIR}/shaders\")
