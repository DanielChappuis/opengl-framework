# Minimum cmake version required
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project configuration
PROJECT(OPENGLFRAMEWORK)

# Where to build the library
SET(LIBRARY_OUTPUT_PATH lib/)

# Where to find the module to find special packages/libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Options
OPTION(COMPILE_DEMO "Select this if you want to build the demo executable" OFF)

# Find OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
if(OPENGL_FOUND)
   MESSAGE("OpenGL found")
else()
   MESSAGE("OpenGL not found")
endif()

# Find the FREEGLUT library
FIND_PATH(FREEGLUT_INCLUDE_DIR NAMES GL/freeglut.h
          /usr/include/GL
          )
FIND_LIBRARY(FREEGLUT_LIBRARY NAMES freeglut glut)
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREEGLUT DEFAULT_MSG FREEGLUT_LIBRARY FREEGLUT_INCLUDE_DIR)
IF(FREEGLUT_FOUND)
	MESSAGE("Freeglut found")
    SET(FREEGLUT_LIBRARIES ${FREEGLUT_LIBRARY})
    SET(FREEGLUT_INCLUDE_DIRS ${FREEGLUT_INCLUDE_DIR})
else()
    MESSAGE("Freeglut not found (required to build the examples)")
    SET(FREEGLUT_LIBRARIES)
    SET(FREEGLUT_INCLUDE_DIRS)
endif()

# Find the GLEW library
FIND_PACKAGE(GLEW REQUIRED)
if(GLEW_FOUND)
   MESSAGE("GLEW found")
else()
   MESSAGE("GLEW not found")
endif()

# Headers
INCLUDE_DIRECTORIES(src)

# Library configuration
file (
  GLOB_RECURSE
  OPENGLFRAMEWORK_SOURCES_FILES
  src/*
)

# Require the opengl-framework code to be compiled in a static library
ADD_LIBRARY (
   openglframework
   STATIC
   ${OPENGLFRAMEWORK_SOURCES_FILES}
)

TARGET_LINK_LIBRARIES(openglframework ${GLEW_LIBRARIES} ${OPENGL_LIBRARY} ${FREEGLUT_LIBRARY})

# If we need to compile the examples
IF (COMPILE_DEMO)
   add_subdirectory(demo/)
ENDIF (COMPILE_DEMO)
