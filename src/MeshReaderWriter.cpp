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
#include "MeshReaderWriter.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <cctype>
#include <map>
#include <algorithm>

using namespace openglframework;
using namespace std;

// Constructor
MeshReaderWriter::MeshReaderWriter() {

}

// Load a mesh from a file and returns true if the mesh has been sucessfully loaded
bool MeshReaderWriter::readMeshFromFile(const std::string& filename, Mesh& meshToCreate) {

    // Get the extension of the file
    uint startPosExtension = filename.find_last_of(".");
    string extension = filename.substr(startPosExtension+1);

    // Load the file using the correct method
    if (extension == "obj") {
        return loadOBJFile(filename, meshToCreate);
    }
    else {

        // Display an error message and throw an exception
        string errorMessage("Error : the MeshLoader class cannot load a file with the extension .");
        errorMessage += extension;
        std::cerr << errorMessage << std::endl;
        throw std::exception(errorMessage.c_str());
    }
}

// Write a mesh to a file
bool MeshReaderWriter::writeMeshToFile(const std::string& filename, const Mesh& meshToWrite) {

    // TODO : Implement this method

    throw std::exception("Error : This method is not implemented yet !");
}

// Load an OBJ file with a triangular or quad mesh
bool MeshReaderWriter::loadOBJFile(const string &filename, Mesh& meshToCreate) {

    // Open the file
    std::ifstream meshFile(filename.c_str());
    assert(meshFile.is_open());
    if(!meshFile.is_open()) return false;

    std::string buffer;
    string line, tmp;
    int id1, id2, id3, id4;
    int nId1, nId2, nId3, nId4;
    int tId1, tId2, tId3, tId4;
    float v1, v2, v3;
    int found1, found2;
    std::vector<bool> isQuad;
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<uint> verticesIndices;
    std::vector<uint> normalsIndices;
    std::vector<uint> uvsIndices;

    // ---------- Collect the data from the file ---------- //

    // For each line of the file
    while(std::getline(meshFile, buffer)) {

        std::istringstream lineStream(buffer);
        std::string word;
        lineStream >> word;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if(word == "usemtl") {  // Material definition

            // Loading of MTL file is not implemented

        }
        else if(word == "v") {  // Vertex position
            sscanf_s(buffer.c_str(), "%*s %f %f %f", &v1, &v2, &v3);
            vertices.push_back(Vector3(v1, v2, v3));
        }
        else if(word == "vt") { // Vertex texture coordinate
            sscanf_s(buffer.c_str(), "%*s %f %f", &v1, &v2);
            uvs.push_back(Vector2(v1,v2));
        }
        else if(word == "vn") { // Vertex normal
            sscanf_s(buffer.c_str(), "%*s %f %f %f", &v1, &v2, &v3);
            normals.push_back(Vector3(v1 ,v2, v3));
        }
        else if (word == "f") { // Face
            line = buffer;
            found1 = (int)line.find("/");
            bool isFaceQuad = false;

            // If the face definition only contains vertices (and not texture coordinates)
            if(found1 == string::npos) {
                int nbVertices = sscanf_s(buffer.c_str(), "%*s %d %d %d %d", &id1, &id2, &id3, &id4);
                if (nbVertices == 4) isFaceQuad = true;
            }
            else {  // If the face definition contains vertices and texture coordinates

                //get the part of the string until the second index
                tmp = line.substr(found1+1);
                found2 = (int)tmp.find(" ");
                tmp = tmp.substr(0,found2);
                found2 = (int)tmp.find("/");

                // If the face definition is of the form "f vert1/textcoord1 vert2/textcoord2 ..."
                if(found2 == string::npos) {
                    int n = sscanf_s(buffer.c_str(), "%*s %d/%d %d/%d %d/%d %d/%d", &id1, &tId1, &id2, &tId2, &id3, &tId3, &id4, &tId4);
                    if (n == 8) isFaceQuad = true;
                    uvsIndices.push_back(tId1-1);
                    uvsIndices.push_back(tId2-1);
                    uvsIndices.push_back(tId3-1);
                    if (isFaceQuad) uvsIndices.push_back(tId4-1);
                }
                else {
                    tmp = line.substr(found1+1);
                    found2 = (int)tmp.find("/");

                    // If the face definition is of the form "f vert1/normal1 vert2/normal2 ..."
                    if(found2 == 0) {
                        int n = sscanf_s(buffer.c_str(), "%*s %d//%d %d//%d %d//%d %d//%d", &id1, &nId1, &id2, &nId2, &id3, &nId3, &id4, &nId4);
                        if (n == 8) isFaceQuad = true;
                    }
                    // If the face definition is of the form "f vert1/textcoord1/normal1 ..."
                    else {
                        int n = sscanf_s(buffer.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &id1, &tId1, &nId1, &id2, &tId2, &nId2, &id3, &tId3, &nId3, &id4, &tId4, &nId4);
                        if (n == 12) isFaceQuad = true;
                        uvsIndices.push_back(tId1-1);
                        uvsIndices.push_back(tId2-1);
                        uvsIndices.push_back(tId3-1);
                        if (isFaceQuad) uvsIndices.push_back(tId4-1);
                    }
                    normalsIndices.push_back(nId1-1);
                    normalsIndices.push_back(nId2-1);
                    normalsIndices.push_back(nId3-1);
                    if (isFaceQuad) normalsIndices.push_back(nId4-1);
                }
            }
            verticesIndices.push_back(id1-1);
            verticesIndices.push_back(id2-1);
            verticesIndices.push_back(id3-1);
            if (isFaceQuad) verticesIndices.push_back((id4-1));
            isQuad.push_back(isFaceQuad);
        }
    }

    assert(!verticesIndices.empty());
    assert(normalsIndices.empty() || normalsIndices.size() == verticesIndices.size());
    assert(uvsIndices.empty() || uvsIndices.size() == verticesIndices.size());
    meshFile.close();

    // ---------- Merge the data that we have collected from the file ---------- //

    // Destroy the current mesh
    meshToCreate.destroy();

    // Mesh data
    vector<std::vector<uint> > meshIndices;
    vector<Vector3> meshVertices;
    vector<Vector3> meshNormals;
    vector<Vector2> meshUVs;

    std::vector<unsigned int> indicesMesh(verticesIndices.size());

    typedef std::map<VertexMergingData, unsigned int, VertexMergingDataComparison> VertexMerginMap;
    VertexMerginMap vertexMergingMap;
    VertexMerginMap::iterator it;
    for(unsigned int i = 0; i < verticesIndices.size(); ++i)
    {
        VertexMergingData vmd;
        vmd.indexPosition = verticesIndices[i];
        if(!normalsIndices.empty()) vmd.indexNormal = normalsIndices[i];
        if(!uvsIndices.empty()) vmd.indexUV = uvsIndices[i];
        it = vertexMergingMap.find(vmd);
        if(it != vertexMergingMap.end())
        {
            indicesMesh[i] = it->second;
        }
        else
        {
            indicesMesh[i] = (unsigned int)meshVertices.size();
            vertexMergingMap[vmd] = (unsigned int)meshVertices.size();
            meshVertices.push_back(vertices[verticesIndices[i]]);
            if(!normalsIndices.empty() && !normals.empty()) meshNormals.push_back(normals[normalsIndices[i]]);
            if(!uvsIndices.empty()) meshUVs.push_back(uvs[uvsIndices[i]]);
        }
    }
    assert(!meshVertices.empty());
    assert(meshNormals.empty() || meshNormals.size() == meshVertices.size());
    assert(meshUVs.empty() || meshUVs.size() == meshVertices.size());

    // We cannot load mesh with several parts for the moment
    uint meshPart = 0;

    // Fill in the vertex indices
    // We also triangulate each quad face
    meshIndices.push_back(std::vector<uint>());
    for(int i = 0, j = 0; i < indicesMesh.size(); j++) {

        // Get the current vertex IDs
        uint i1 = indicesMesh[i];
        uint i2 = indicesMesh[i+1];
        uint i3 = indicesMesh[i+2];

        // If the current vertex not in a quad (it is part of a triangle)
        if (!isQuad[j]) {

            meshIndices[meshPart].push_back(i1);
            meshIndices[meshPart].push_back(i2);
            meshIndices[meshPart].push_back(i3);

            i+=3;
        }
        else {  // If the current vertex is in a quad

            Vector3 v1 = meshVertices[i1];
            Vector3 v2 = meshVertices[i2];
            Vector3 v3 = meshVertices[i3];
            uint i4 = indicesMesh[i+3];
            Vector3 v4 = meshVertices[i4];

            Vector3 v13 = v3-v1;
            Vector3 v12 = v2-v1;
            Vector3 v14 = v4-v1;

            float a1 = v13.dot(v12);
            float a2 = v13.dot(v14);
            if((a1 >= 0 && a2 <= 0) || (a1 <= 0 && a2 >= 0)) {
                meshIndices[meshPart].push_back(i1);
                meshIndices[meshPart].push_back(i2);
                meshIndices[meshPart].push_back(i3);
                meshIndices[meshPart].push_back(i1);
                meshIndices[meshPart].push_back(i3);
                meshIndices[meshPart].push_back(i4);
            }
            else {
                meshIndices[meshPart].push_back(i1);
                meshIndices[meshPart].push_back(i2);
                meshIndices[meshPart].push_back(i4);
                meshIndices[meshPart].push_back(i2);
                meshIndices[meshPart].push_back(i3);
                meshIndices[meshPart].push_back(i4);
            }
            i+=4;
        }
    }

    // Set the data to the mesh
    meshToCreate.setIndices(meshIndices);
    meshToCreate.setVertices(meshVertices);
    meshToCreate.setNormals(meshNormals);
    meshToCreate.setUVs(meshUVs);

    return true;
}
