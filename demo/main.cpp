/********************************************************************************
* OpenGL-Framework                                                              *
* Copyright (c) 2013 Daniel Chappuis                                            *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

// Libraries
#include "Scene.h"

// Declarations
void simulate();
void display();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void init();

// Namespaces
using namespace openglframework;

// Global variables
Viewer* viewer;
Scene* scene;

// Main function
int main(int argc, char** argv) {

    // Create and initialize the Viewer
    viewer = new Viewer();
    Vector2 windowsSize = Vector2(600, 400);
    Vector2 windowsPosition = Vector2(100, 100);
    bool initOK = viewer->init(argc, argv, "OpenGL Framework Demo", windowsSize, windowsPosition);
    if (!initOK) return 1;

    // Create the scene
    scene = new Scene(viewer);

    init();

    // Glut Idle function that is continuously called
    glutIdleFunc(simulate);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    // Glut main looop
    glutMainLoop();

    delete viewer;
    delete scene;

    return 0;
}

// Simulate function
void simulate() {

    // Display the scene
    display();
}

// Initialization
void init() {

    // Define the background color (black)
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

// Reshape function
void reshape(int width, int height) {
    viewer->reshape(width, height);
}

// Called when a mouse button event occurs
void mouseButton(int button, int state, int x, int y) {
    viewer->mouseButtonEvent(button, state, x, y);
}

// Called when a mouse motion event occurs
void mouseMotion(int x, int y) {
    viewer->mouseMotionEvent(x, y);
}

// Display the scene
void display() {

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->display();

    glutSwapBuffers();

    // Check the OpenGL errors
    Viewer::checkOpenGLErrors();
}
