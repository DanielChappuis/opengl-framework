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

// Librairies
#include "TextureReaderWriter.h"
#include <string>

using namespace openglframework;
using namespace std;

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

// Load a texture from a file
bool TextureReaderWriter::loadTextureFromFile(const std::string& filename,
                                              Texture2D& textureToCreate)
                                              throw(runtime_error, invalid_argument){

    // Get the extension of the file
    uint startPosExtension = filename.find_last_of(".");
    string extension = filename.substr(startPosExtension+1);

    // Load the file using the correct method
    if (extension == "tga") {
        return readTGAPicture(filename, textureToCreate);
    }
    else {

        // Display an error message and throw an exception
        string errorMessage("Error : the TextureLoader class cannot load a file with the extension .");
        errorMessage += extension;
        std::cerr << errorMessage << std::endl;
        throw std::invalid_argument(errorMessage.c_str());
    }
}

// Write a texture to a file
void TextureReaderWriter::writeTextureToFile(const std::string& filename,
                                             const Texture2D& texture)
                                             throw(runtime_error, invalid_argument){

    // Get the extension of the file
    uint startPosExtension = filename.find_last_of(".");
    string extension = filename.substr(startPosExtension+1);

    // Write the file using the correct method
    if (extension == "tga") {
        writeTGAPicture(filename, texture);
    }
    else {

        // Display an error message and throw an exception
        string errorMessage("Error : the TextureReaderWriter class cannot write a file with the extension .");
        errorMessage += extension;
        std::cerr << errorMessage << std::endl;
        throw std::invalid_argument(errorMessage.c_str());
    }
}

// Load a TGA picture
bool TextureReaderWriter::readTGAPicture(const std::string &filename,
                                         Texture2D& textureToCreate) throw(runtime_error) {

    // Open the file
    std::ifstream stream(filename.c_str(), std::ios::binary);

    // If we cannot open the file
    if(!stream.is_open()) {

        // Throw an exception
        throw std::runtime_error("Error : Cannot open the file " + filename);
    }

    TGA_HEADER header;
    stream.read((char *)(&header), sizeof(TGA_HEADER));
    assert(header.width <= 4096 && header.width <= 4096 &&
           header.imagetype == 2 && header.bits == 24);

    // Read the file
    uint width = header.width;
    uint height = header.width;
    uint sizeImg = width*height;
    char* data = new char[sizeImg*3];
    assert(data);
    stream.read(data, sizeImg*3);
    for(uint i = 0; i < sizeImg; i++) {
        unsigned pos = i*3;
        unsigned char red = data[pos];
        data[pos] = data[pos + 2];
        data[pos + 2] = red;
    }

    // Close the stream
    stream.close();

    // Create the OpenGL texture using the picture data from the file
    textureToCreate.create(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Free the data memory
    delete[] data;
}


// Write a TGA picture
void TextureReaderWriter::writeTGAPicture(const std::string& filename,
                                          const Texture2D& texture) throw(runtime_error) {
    assert(texture.getID() != 0);

    // Bind the corresponding texture
    glBindTexture(GL_TEXTURE_2D, texture.getID());

    uint sizeImg = texture.getWidth() * texture.getHeight();

    // Get the bytes form the OpenGL texture
    char* data = new char[sizeImg * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Open the file
    std::ofstream stream(filename.c_str(), std::ios::binary);

    // If the file cannot be opened
    if(!stream.is_open()) {

        // Throw an exception
        throw std::runtime_error("Error : Cannot create/access the file " + filename);
    }

    // Fill in the TGA header
    TGA_HEADER header;
    header.identsize = 0;           // size of ID field that follows 18 byte header (0 usually)
    header.colourmaptype = 0;       // type of colour map 0=none, 1=has palette
    header.imagetype = 2;           // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
    header.colourmapstart = 0;                  // first colour map entry in palette
    header.colourmaplength = 0;                 // number of colours in palette
    header.colourmapbits=0;                     // number of bits per palette entry 15,16,24,32
    header.xstart = 0;                          // image x origin
    header.ystart = 0;                          // image y origin
    header.width = (short)texture.getWidth();   // image width in pixels
    header.height = (short)texture.getHeight(); // image height in pixels
    header.bits = 24;                           // image bits per pixel 8,16,24,32
    header.descriptor = 0;                      // image descriptor bits (vh flip bits)

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

    // Close the file
    stream.close();

    // Delete the data
    delete [] data;

    // Unbind the corresponding texture
    glBindTexture(GL_TEXTURE_2D, 0);
}
