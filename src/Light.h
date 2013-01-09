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

#ifndef LIGHT_H
#define LIGHT_H

// Libraries
#include "maths/Color.h"
#include "maths/Vector3.h"
#include "Object3D.h"
#include "Texture2D.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include <GL/glew.h>

namespace openglframework {

// Class Light
class Light : public Object3D {

    private:

        // -------------------- Attributes -------------------- //

        // OpenGL light ID
        GLuint mLightID;

        // Diffuse color of the light
        Color mDiffuseColor;

        // Specular color of the light
        Color mSpecularColor;

        // True if the light is active
        bool mIsActive;

        // Shadow map associated with this light
        Texture2D mShadowMap;

        // Framebuffer object to render the shadow map
        FrameBufferObject mFBOShadowMap;

        // Shader to render the depth of the scene into a texture
        Shader mDepthShader;

    public:

        // -------------------- Methods -------------------- //

        // Constructor
        Light(GLuint id);

        // Constructor
        Light(GLuint id, Color diffuseColor, Color specularColor);

        // Destructor
        virtual ~Light();

        // Return the diffuse color
        Color getDiffuseColor() const;

        // Return the specular color
        Color getSpecularColor() const;

        // Return true if the light is active
        bool getIsActive() const;

        // Initialize the light
        void init();

        // Enable the light
        void enable();

        // Disable the light
        void disable();

        // Create a shadow map associated with this light
        bool createShadowMap(uint width, uint height);

        // Call this method before rendering the scene for the shadow map computation
        void startRenderingShadowMap();

        // Call this method after having rendered the scene for the shadow map computation
        void stopRenderingShadowMap();

        // Destroy the shadow map associated with this light
        void destroyShadowMap();
};

// Return the diffuse color
inline Color Light::getDiffuseColor() const {
    return mDiffuseColor;
}

// Return the specular color
inline Color Light::getSpecularColor() const {
    return mSpecularColor;
}

// Return true if the light is active
inline bool Light::getIsActive() const {
    return mIsActive;
}

// Enable the light
inline void Light::enable() {

    // Enable the light
    glEnable(GL_LIGHTING);
    glEnable(mLightID);
}

// Disable the light
inline void Light::disable() {

    // Disable the light
    glDisable(mLightID);
}

// Destroy the shadow map associated with this light
inline void Light::destroyShadowMap() {
    mShadowMap.destroy();
    mFBOShadowMap.destroy();
    mDepthShader.destroy();
}

// Call this method before rendering the scene for the shadow map computation
inline void Light::startRenderingShadowMap() {
    assert(mShadowMap.getID());
}

// Call this method after having rendered the scene for the shadow map computation
inline void Light::stopRenderingShadowMap() {
    assert(mShadowMap.getID());
}

}

#endif
