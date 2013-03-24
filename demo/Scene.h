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

#ifndef SCENE_H
#define SCENE_H

// Libraries
#include <openglframework.h>

// Class Scene
class Scene {

    private :

        // -------------------- Attributes -------------------- //

        // Pointer to the viewer
        openglframework::GlutViewer* mViewer;

        // Light 0
        openglframework::Light mLight0;

        // Mesh
        openglframework::Mesh mMesh;

        // Phong shader
        openglframework::Shader mPhongShader;

        // -------------------- Methods -------------------- //

        // Render the mesh
        void renderMesh();

    public:

        // -------------------- Methods -------------------- //

        // Constructor
        Scene(openglframework::GlutViewer* viewer);

        // Destructor
        ~Scene();

        // Display the scene
        void display();
};

#endif
