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
#include "Texture2D.h"
#include <fstream>
#include <iostream>
#include <string>

// Namespaces
using namespace openglframework;

// Constructor
Texture2D::Texture2D() : mID(0), mLayer(0), mWidth(0), mHeight(0) {

}

// Constructor
Texture2D::Texture2D(uint width, uint height, uint internalFormat, uint format, uint type)
          : mID(0), mLayer(0), mWidth(0), mHeight(0){

    // Create the texture
    create(width, height, internalFormat, format, type);
}

// Destructor
Texture2D::~Texture2D() {

}

// Create the texture
void Texture2D::create(uint width, uint height, uint internalFormat, uint format, uint type,
            void* data) throw(std::invalid_argument) {

    assert(width <= 4096 && height <= 4096);

    // Destroy the current texture
    destroy();

    mWidth = width;
    mHeight = height;

    switch(format) {
        case GL_RED:    mNbChannels = 1; break;
        case GL_RG:     mNbChannels = 2; break;
        case GL_RGB:    mNbChannels = 3; break;
        case GL_BGR:    mNbChannels = 3; break;
        case GL_RGBA:   mNbChannels = 4; break;
        case GL_BGRA:   mNbChannels = 4; break;
        default:
            // Throw an exception and display an error message
            std::string errorMessage("Error : Wrong OpenGL texture format");
            std::cerr << errorMessage << std::endl;
            throw std::invalid_argument(errorMessage);
    }

    // Create the OpenGL texture
    glGenTextures(1, &mID);
    assert(mID != 0);
    glBindTexture(GL_TEXTURE_2D, mID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, type, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Destroy the texture
void Texture2D::destroy() {
    if (mID != 0) {
        glDeleteTextures(1, &mID);
        mID = 0;
        mLayer = 0;
        mWidth = 0;
        mHeight = 0;
    }
}
