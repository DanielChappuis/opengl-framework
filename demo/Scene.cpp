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

// Namespaces
using namespace openglframework;

// Constructor
Scene::Scene(Viewer* viewer) : mViewer(viewer), mLight0(0),
                               mPhongShader("shaders/phong.vert",
                                            "shaders/phong.frag"){

    // Move the light 0
    mLight0.translateWorld(Vector3(15, 15, 15));

    // Load the mesh
    bool isMeshLoaded = mMesh.loadFromFile("torus.obj");
    assert(isMeshLoaded);

    // Calculate the normals of the mesh
    mMesh.calculateNormals();

    // Calculate the bounding box of the mesh
    Vector3 min, max;
    mMesh.calculateBoundingBox(min, max);

    // Compute the radius and the center of the scene
    float radius = 0.5f * (min - max).length();
    Vector3 center = 0.5f * (min + max);

    // Set the center of the scene
    mViewer->setScenePosition(center, radius);
}

// Destructor
Scene::~Scene() {

}

// Display the scene
void Scene::display() {

    // Render the mesh
    renderMesh();

    // Display a cube
    //glutSolidCube(5.0);
}

// Render the mesh
void Scene::renderMesh() {

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    // Bind the shader
    mPhongShader.bind();

    // Set the variables of the shader
    const Camera& camera = mViewer->getCamera();
    Matrix4 matrixIdentity;
    matrixIdentity.setToIdentity();
    mPhongShader.setVector3Uniform("cameraWorldPosition", mViewer->getCamera().getOrigin());
    mPhongShader.setMatrix4x4Uniform("modelToWorldMatrix", mMesh.getTransformMatrix());
    mPhongShader.setMatrix4x4Uniform("worldToCameraMatrix", camera.getTransformMatrix().inverse());
    mPhongShader.setMatrix4x4Uniform("projectionMatrix", camera.getProjectionMatrix());
    mPhongShader.setVector3Uniform("lightWorldPosition", mLight0.getOrigin());
    mPhongShader.setVector3Uniform("lightAmbientColor", Vector3(0.3f, 0.3f, 0.3f));
    Color& diffCol = mLight0.getDiffuseColor();
    Color& specCol = mLight0.getSpecularColor();
    mPhongShader.setVector3Uniform("lightDiffuseColor", Vector3(diffCol.r, diffCol.g, diffCol.b));
    mPhongShader.setVector3Uniform("lightSpecularColor", Vector3(specCol.r, specCol.g, specCol.b));
    mPhongShader.setFloatUniform("shininess", 60.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if (mMesh.hasUVTextureCoordinates()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glVertexPointer(3, GL_FLOAT, 0, mMesh.getVerticesPointer());
    glNormalPointer(GL_FLOAT, 0, mMesh.getNormalsPointer());

    // For each part of the mesh
    for (uint i=0; i<mMesh.getNbParts(); i++) {
        glDrawElements(GL_TRIANGLES, mMesh.getNbFaces(i) * 3,
                       GL_UNSIGNED_INT, mMesh.getIndicesPointer());
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (mMesh.hasUVTextureCoordinates()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    // Unbind the shader
    mPhongShader.unbind();
}
