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

// Namespaces
using namespace openglframework;

// TGA file Header
#pragma pack(push, 1)
typedef struct {
    unsigned char  identsize;           // size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;       // type of colour map 0=none, 1=has palette
    unsigned char  imagetype;           // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmapstart;               // first colour map entry in palette
    short colourmaplength;              // number of colours in palette
    unsigned char  colourmapbits;       // number of bits per palette entry 15,16,24,32

    short xstart;                       // image x origin
    short ystart;                       // image y origin
    short width;                        // image width in pixels
    short height;                       // image height in pixels
    unsigned char  bits;                // image bits per pixel 8,16,24,32
    unsigned char  descriptor;          // image descriptor bits (vh flip bits)

    // pixel data follows header
} TGA_HEADER;
#pragma pack(pop)

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

    // Destroy the texture
    destroy();
}

// Create the texture
void Texture2D::create(uint width, uint height, uint internalFormat, uint format, uint type,
            void* data) {

    assert(width <= 4096 && height <= 4096);

    // Destroy the current texture
    destroy();

    mWidth = width;
    mHeight = height;

    // Create the OpenGL texture
    glGenTextures(1, &mID);
    assert(mID != 0);
    glBindTexture(GL_TEXTURE_2D, mID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Create a texture from a file (TGA picture file)
void Texture2D::createFromFile(const std::string& filename) {

    // Read the bytes from the TGA file
    char* bytes = readTGABytes(filename, mWidth, mHeight);

    // Create the OpenGL texture using the TGA bytes
    create(mWidth, mHeight, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, bytes);
}

// Write the texture into a file (TGA picture file)
void Texture2D::writeToFile(const std::string& filename) {
    assert(mID != 0);
    glBindTexture(GL_TEXTURE_2D, mID);
    uint sizeImg = mWidth * mHeight;

    // Get the bytes form the OpenGL texture
    char* data = new char[sizeImg * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Open the file
    std::ofstream stream(filename.c_str(), std::ios::binary);
    assert(stream.is_open());
    if(!stream.is_open()) return;

    // Fill in the TGA header
    TGA_HEADER header;
    header.identsize = 0;           // size of ID field that follows 18 byte header (0 usually)
    header.colourmaptype = 0;       // type of colour map 0=none, 1=has palette
    header.imagetype = 2;           // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
    header.colourmapstart = 0;      // first colour map entry in palette
    header.colourmaplength = 0;     // number of colours in palette
    header.colourmapbits=0;         // number of bits per palette entry 15,16,24,32
    header.xstart = 0;              // image x origin
    header.ystart = 0;              // image y origin
    header.width = (short)mWidth;   // image width in pixels
    header.height = (short)mHeight; // image height in pixels
    header.bits = 24;               // image bits per pixel 8,16,24,32
    header.descriptor = 0;          // image descriptor bits (vh flip bits)

    // Write the header to the file
    stream.write((char*)(&header), sizeof(TGA_HEADER));

    // Write the bytes to the file
    for(uint i = 0; i < sizeImg; i++) {
        unsigned pos = i*3;
        unsigned char red = data[pos];
        data[pos] = data[pos + 2];
        data[pos + 2] = red;
    }
    stream.write(data, sizeImg*3);
    stream.close();
    delete [] data;
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

// Return the bytes of a TGA file
char* Texture2D::readTGABytes(const std::string& filename, uint& width, uint& height) {

    // Open the file
    std::ifstream stream(filename.c_str(), std::ios::binary);
    assert(stream.is_open());
    if(!stream.is_open()) return NULL;

    TGA_HEADER header;
    stream.read((char *)(&header), sizeof(TGA_HEADER));
    assert(header.width <= 4096 && header.width <= 4096 &&
           header.imagetype == 2 && header.bits == 24);

    // Read the file
    width = header.width;
    height = header.width;
    uint sizeImg = width*height;
    char *data = new char[sizeImg*3];
    stream.read(data, sizeImg*3);
    for(uint i = 0; i < sizeImg; i++) {
        unsigned pos = i*3;
        unsigned char red = data[pos];
        data[pos] = data[pos + 2];
        data[pos + 2] = red;
    }

    // Close the stream
    stream.close();

    // Return the bytes
    return data;
}
